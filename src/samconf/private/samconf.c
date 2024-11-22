// SPDX-License-Identifier: MIT
#include "samconf/samconf.h"

#include <libgen.h>
#include <safu/common.h>
#include <safu/log.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

#include "samconf/config_backend.h"
#include "samconf/crypto_utils.h"
#include "samconf/dummy_backend.h"
#include "samconf/env_backend.h"
#include "samconf/json_backend.h"
#include "samconf/signature.h"

static const samconfConfigBackendOps_t *const samconfBackendOps[BACKEND_COUNT] = {
#ifdef SAMCONF_ENABLE_CONFIG_BACKEND_JSON
    [BACKEND_JSON] = &samconfJsonOps,
#endif

#ifdef SAMCONF_ENABLE_CONFIG_BACKEND_ENV
    [BACKEND_ENV] = &samconfEnvOps,
#endif

    [BACKEND_DUMMY] = &samconfDummyOps,
};

NOINLINE const samconfConfigBackendOps_t *samconfGetBackendOps(samconfBackendOptions_t idx) {
    const samconfConfigBackendOps_t *backend = &samconfDummyOps;
    if (idx < BACKEND_COUNT && samconfBackendOps[idx] != NULL) {
        backend = samconfBackendOps[idx];
    }
    return backend;
}

NOINLINE samconfConfigStatusE_t samconfLookupBackend(const char *location, samconfConfigBackend_t **backend) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    bool isSupported = false;
    const samconfConfigBackendOps_t *backendOps = NULL;

    for (samconfBackendOptions_t idx = BACKEND_JSON; idx < BACKEND_COUNT; ++idx) {
        backendOps = samconfGetBackendOps(idx);
        if (backendOps->supports(location, &isSupported) == SAMCONF_CONFIG_OK && isSupported) {
            status = samconfConfigBackendNew(backend, backendOps);
            break;
        }
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfVerifySignature(const char *location) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    char *data, *signature;
    size_t signatureLen;
    ssize_t dataLen;

    status = samconfSignatureLoaderGetSignatureFor(location, &signature, &signatureLen);
    if (status == SAMCONF_CONFIG_OK) {
        dataLen = safuReadFileToString(location, -1, &data);
        if (dataLen >= 0) {
            status = samconfCryptoUtilsVerify(data, dataLen, signature);
            if (status != SAMCONF_CONFIG_OK) {
                safuLogErrF("Failed to verify signature of %s.\n", location);
            }

            free(data);
        } else {
            safuLogErrF("Failed to read signature from %s: Read failed.\n", location);
        }

        free(signature);
    } else if (status != SAMCONF_CONFIG_SIGNATURE_NOT_FOUND) {
        safuLogErrF("Failed to load signature: %s.sig.\n", location);
    }

    return status;
}

samconfConfigStatusE_t samconfLoad(const char *location, bool enforceSignature, samconfConfig_t **const config) {
    samconfConfigStatusE_t status, secStatus;
    samconfConfigBackend_t *backend = NULL;
    samconfConfig_t *newConfig = NULL;
    bool isSigned;

    if (config == NULL) {
        safuLogErr("Invalid Call to samconfLoad, uninitialized configuration");
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfVerifySignature(location);
    isSigned = (status == SAMCONF_CONFIG_OK);
    if (isSigned || ((status == SAMCONF_CONFIG_SIGNATURE_NOT_FOUND) && !enforceSignature)) {
        status = samconfLookupBackend(location, &backend);
        if (status == SAMCONF_CONFIG_OK) {
            status = backend->ops->open(location, backend);
            if (status == SAMCONF_CONFIG_OK) {
                status = backend->ops->load(backend, isSigned, &newConfig);
                if (status != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to load configuration %s\n", location);
                }

                secStatus = backend->ops->close(backend);
                if (secStatus != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to close configuration %s\n", location);
                    status = secStatus;
                }
            } else {
                safuLogErrF("Failed to open configuration %s.\n", location);
            }

            secStatus = samconfConfigBackendDelete(backend);
            if (secStatus != SAMCONF_CONFIG_OK) {
                safuLogErrF("Failed to delete backend for %s\n", location);
                status = secStatus;
            }

            if (status != SAMCONF_CONFIG_OK && newConfig != NULL) {
                secStatus = samconfConfigDelete(newConfig);
                if (secStatus != SAMCONF_CONFIG_OK) {
                    safuLogErrF("Failed to cleanup after issues while loading %s\n", location);
                }
            }
        } else {
            safuLogErrF("Failed to lookup backend for %s.\n", location);
        }
    } else {
        safuLogErrF("Failed to verify signature for %s.", location);
    }

    if (status == SAMCONF_CONFIG_OK) {
        *config = newConfig;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigNew(samconfConfig_t **const config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        *config = safuAllocMem(NULL, sizeof(samconfConfig_t));
        if (*config != NULL) {
            status = samconfConfigInit(*config);
        }
    }

    return status;
}

samconfConfigStatusE_t samconfConfigInit(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        memset(config, 0, sizeof(*config));
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigDeleteMembers(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    if (config) {
        free(config->key);

        if (config->type == SAMCONF_CONFIG_VALUE_STRING) {
            free(config->value.string);
        }

        for (size_t i = 0; i < config->childCount; i++) {
            if (samconfConfigDelete(config->children[i]) == SAMCONF_CONFIG_ERROR) {
                status = SAMCONF_CONFIG_ERROR;
            }
        }

        if (config->childCount > 0) {
            free(config->children);
        }
    } else {
        status = SAMCONF_CONFIG_ERROR;
    }

    return status;
}

NOINLINE samconfConfigStatusE_t samconfConfigDelete(samconfConfig_t *config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    status = samconfConfigDeleteMembers(config);

    free(config);

    return status;
}

samconfConfigStatusE_t samconfConfigAdd(samconfConfig_t *parent, samconfConfig_t *child) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    if (!parent || !child) {
        status = SAMCONF_CONFIG_ERROR;
        return status;
    }

    parent->children = safuAllocMem(parent->children, sizeof(samconfConfig_t *) * (parent->childCount + 1));

    if (!parent->children) {
        status = SAMCONF_CONFIG_ERROR;
        return status;
    }

    parent->childCount += 1;

    parent->children[parent->childCount - 1] = child;

    child->parent = parent;

    return status;
}

static samconfConfigStatusE_t _add_to_new_path(const char *existingPath, const char *segment, char **newPath) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    size_t strLen = 0;
    int ret = 0;

    if (existingPath == NULL || segment == NULL) {
        safuLogErr("invalid parameters");
    } else {
        strLen = strlen(segment) + strlen(existingPath);
        *newPath = safuAllocMem(NULL, strLen + 2);
        if (*newPath == NULL) {
            safuLogErr("SafuAllocMem failed");
        } else {
            if (existingPath[0] == '\0') {
                ret = snprintf(*newPath, strLen + 2, "%s", segment);
            } else {
                ret = snprintf(*newPath, strLen + 2, "%s/%s", segment, existingPath);
            }
            if (ret < 0) {
                safuLogErr("snprintf failed");
                free(*newPath);
            } else {
                result = SAMCONF_CONFIG_OK;
            }
        }
    }
    return result;
}

samconfConfigStatusE_t samconfGetParentPath(const samconfConfig_t *config, const char **path) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *parent = NULL;
    char *rootKey = "root";
    char *existingPath = "";
    char *newPath = NULL;

    if (config == NULL || path == NULL) {
        safuLogErr("invalid parameters");
    } else {
        if (strcmp(config->key, rootKey) == 0) {
            *path = strdup(rootKey);
            result = SAMCONF_CONFIG_OK;
        } else {
            result = _add_to_new_path(existingPath, config->key, &newPath);
            if (result == SAMCONF_CONFIG_ERROR) {
                safuLogErrF("adding %s to path failed", config->key);
            } else if (newPath == NULL) {
                safuLogErr("created path is null");
                result = SAMCONF_CONFIG_ERROR;
            } else {
                parent = config->parent;
                while (strcmp(parent->key, rootKey) != 0) {
                    existingPath = strdup(newPath);
                    if (existingPath == NULL) {
                        safuLogErr("strdup failed");
                        result = SAMCONF_CONFIG_ERROR;
                        free(newPath);
                        break;
                    }
                    free(newPath);
                    newPath = NULL;
                    result = _add_to_new_path(existingPath, parent->key, &newPath);
                    if (result == SAMCONF_CONFIG_ERROR) {
                        safuLogErrF("adding %s to path failed", parent->key);
                        free(existingPath);
                        break;
                    }
                    parent = parent->parent;
                    result = SAMCONF_CONFIG_OK;
                    free(existingPath);
                }
                if (strcmp(parent->key, rootKey) == 0 && result == SAMCONF_CONFIG_OK) {
                    existingPath = strdup(newPath);
                    if (existingPath == NULL) {
                        safuLogErr("strdup failed");
                        result = SAMCONF_CONFIG_ERROR;
                    } else {
                        free(newPath);
                        newPath = NULL;
                        result = _add_to_new_path(existingPath, rootKey, &newPath);
                        if (result == SAMCONF_CONFIG_ERROR) {
                            safuLogErrF("adding %s to path failed", rootKey);
                        } else {
                            *path = newPath;
                            result = SAMCONF_CONFIG_OK;
                        }
                        free(existingPath);
                    }
                }
            }
        }
    }

    return result;
}

samconfConfigStatusE_t samconfCreateIntAt(samconfConfig_t **root, const char *path, int64_t value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *config = NULL;
    char *copyPath = NULL;
    char *key = NULL;

    if (root == NULL || path == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        result = samconfConfigNew(&config);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("samconfConfigNew failed");
        } else {
            copyPath = strdup(path);
            if (copyPath == NULL) {
                safuLogErrF("strdup path %s failed", path);
            } else {
                key = basename(copyPath);
                config->key = strdup(key);
                if (config->key == NULL) {
                    safuLogErrF("strdup key %s failed", key);
                } else {
                    result = samconfConfigSetInt(config, value);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErr("samconfConfigSetInt failed");
                    } else {
                        result = samconfInsertAt(root, path, config);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfInsertAt failed");
                        }
                    }
                }
            }
            free(copyPath);
        }
    }
    return result;
}

samconfConfigStatusE_t samconfCreateRealAt(samconfConfig_t **root, const char *path, double value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *config = NULL;
    char *copyPath = NULL;
    char *key = NULL;

    if (root == NULL || path == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        result = samconfConfigNew(&config);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("samconfConfigNew failed");
        } else {
            copyPath = strdup(path);
            if (copyPath == NULL) {
                safuLogErrF("strdup path %s failed", path);
            } else {
                key = basename(copyPath);
                config->key = strdup(key);
                if (config->key == NULL) {
                    safuLogErrF("strdup key %s failed", key);
                } else {
                    result = samconfConfigSetReal(config, value);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErr("samconfConfigSetReal failed");
                    } else {
                        result = samconfInsertAt(root, path, config);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfInsertAt failed");
                        }
                    }
                }
            }
            free(copyPath);
        }
    }

    return result;
}

samconfConfigStatusE_t samconfCreateBoolAt(samconfConfig_t **root, const char *path, bool value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *config = NULL;
    char *copyPath = NULL;
    char *key = NULL;

    if (root == NULL || path == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        result = samconfConfigNew(&config);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("samconfConfigNew failed");
        } else {
            copyPath = strdup(path);
            if (copyPath == NULL) {
                safuLogErrF("strdup path %s failed", path);
            } else {
                key = basename(copyPath);
                config->key = strdup(key);
                if (config->key == NULL) {
                    safuLogErrF("strdup key %s failed", key);
                } else {
                    result = samconfConfigSetBool(config, value);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErr("samconfConfigSetBool failed");
                    } else {
                        result = samconfInsertAt(root, path, config);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfInsertAt failed");
                        }
                    }
                }
            }
            free(copyPath);
        }
    }
    return result;
}

samconfConfigStatusE_t samconfCreateFromStringAt(samconfConfig_t **root, const char *path, char *value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *config = NULL;
    char *copyPath = NULL;
    char *key = NULL;

    if (root == NULL || path == NULL || value == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        result = samconfConfigNew(&config);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("samconfConfigNew failed");
        } else {
            copyPath = strdup(path);
            if (copyPath == NULL) {
                safuLogErrF("strdup path %s failed", path);
            } else {
                key = basename(copyPath);
                config->key = strdup(key);
                if (config->key == NULL) {
                    safuLogErrF("strdup key %s failed", key);
                } else {
                    result = samconfConfigSetValueFromString(config, value);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErr("samconfConfigSetValueFromString failed");
                    } else {
                        result = samconfInsertAt(root, path, config);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfInsertAt failed");
                        }
                    }
                }
            }
            free(copyPath);
        }
    }
    return result;
}

samconfConfigStatusE_t samconfCreateStringAt(samconfConfig_t **root, const char *path, const char *value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *config = NULL;
    char *copyPath = NULL;
    char *key = NULL;

    if (root == NULL || path == NULL || value == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        result = samconfConfigNew(&config);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("samconfConfigNew failed");
        } else {
            copyPath = strdup(path);
            if (copyPath == NULL) {
                safuLogErrF("strdup path %s failed", path);
            } else {
                key = basename(copyPath);
                config->key = strdup(key);
                if (config->key == NULL) {
                    safuLogErrF("strdup key %s failed", key);
                } else {
                    result = samconfConfigSetString(config, value);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErr("samconfConfigSetString failed");
                    } else {
                        result = samconfInsertAt(root, path, config);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfInsertAt failed");
                        }
                    }
                }
            }
            free(copyPath);
        }
    }
    return result;
}

samconfConfigStatusE_t samconfCopyConfigValue(samconfConfig_t *from, samconfConfig_t *to) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    if (from == NULL || to == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        switch (from->type) {
            case SAMCONF_CONFIG_VALUE_INT:
                result = samconfConfigSetInt(to, from->value.integer);
                break;
            case SAMCONF_CONFIG_VALUE_BOOLEAN:
                result = samconfConfigSetBool(to, from->value.boolean);
                break;
            case SAMCONF_CONFIG_VALUE_REAL:
                result = samconfConfigSetReal(to, from->value.real);
                break;
            default:
                result = samconfConfigSetString(to, from->value.string);
        }
    }

    return result;
}

static void _get_path_to_token(char **pathToToken, char *token) {
    size_t tokenLen = strlen(token);

    if (*pathToToken == NULL) {
        *pathToToken = strdup(token);
    } else {
        size_t pathLen = strlen(*pathToToken);
        size_t newPathLen = pathLen + tokenLen + 2;
        char *newPath = safuAllocMem(NULL, newPathLen);

        snprintf(newPath, newPathLen, "%s/%s", *pathToToken, token);
        free(*pathToToken);
        *pathToToken = newPath;
    }
}

samconfConfigStatusE_t samconfInsertAt(samconfConfig_t **root, const char *path, samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char *pathCopy = NULL;
    char *freePath = NULL;
    char *lastConfig = NULL;
    char *pathToToken = NULL;
    samconfConfig_t *node = NULL;
    samconfConfig_t *parent = NULL;
    const samconfConfig_t *existingNode = NULL;

    if (root == NULL || path == NULL || config == NULL) {
        result = SAMCONF_CONFIG_ERROR;
    } else {
        freePath = pathCopy = strdup(path);

        if (pathCopy == NULL || freePath == NULL) {
            result = SAMCONF_CONFIG_ERROR;
        } else {
            if (pathCopy[0] == '\0') {
                result = SAMCONF_CONFIG_NOT_FOUND;
            } else {
                if (pathCopy[0] == '/') {
                    if (strlen(pathCopy) == 1) {
                        result = SAMCONF_CONFIG_NOT_FOUND;
                    } else {
                        ++pathCopy;
                    }
                }
            }
        }

        if (result == SAMCONF_CONFIG_OK) {
            lastConfig = basename(pathCopy);
            char *token = strsep(&pathCopy, "/");
            parent = *root;

            while (token) {
                _get_path_to_token(&pathToToken, token);
                if (pathToToken != NULL) {
                    result = samconfConfigGet(*root, pathToToken, &existingNode);
                    if (result == SAMCONF_CONFIG_OK) {
                        parent = (samconfConfig_t *)existingNode;
                        if (strcasecmp(pathToToken, token) == 0) {
                            if (token == lastConfig) {
                                safuLogErrF("cannot insert node : %s, already exists", config->key);
                                samconfConfigDelete(config);
                                break;
                            }
                        }
                    } else {
                        if (token == lastConfig) {
                            result = samconfConfigAdd(parent, config);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogErrF("samconfConfigAdd failed to add node : %s", config->key);
                                samconfConfigDelete(config);
                                break;
                            }
                        } else {
                            result = samconfConfigNew(&node);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogErr("samconfConfigNew failed");
                                break;
                            }
                            node->key = strdup(token);
                            node->type = SAMCONF_CONFIG_VALUE_OBJECT;
                            result = samconfConfigAdd(parent, node);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogErrF("samconfConfigAdd failed to add node : %s", node->key);
                                samconfConfigDelete(node);
                                break;
                            }
                            parent = node;
                        }
                    }
                }
                token = strsep(&pathCopy, "/");
            }
            free(pathToToken);
        }
        free(freePath);
    }

    return result;
}

samconfConfigStatusE_t samconfConfigGet(const samconfConfig_t *root, const char *path, const samconfConfig_t **result) {
    size_t childItr = 0;
    size_t childCount = 0;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }
    if (!root->children) {
        return SAMCONF_CONFIG_NOT_FOUND;
    }
    const samconfConfig_t *node = *(root->children);
    const samconfConfig_t *parent = root;
    childCount = root->childCount;

    char *pathCopy, *freePath, *lastConfig;

    // Two Pointers are needed  at start of string
    // to free them later.
    //
    freePath = pathCopy = strdup(path);

    if (!pathCopy || !freePath) {
        return SAMCONF_CONFIG_ERROR;
    }

    if (pathCopy[0] == '\0') {
        free(freePath);
        return SAMCONF_CONFIG_NOT_FOUND;
    }

    if (pathCopy[0] == '/') {
        if (strlen(pathCopy) == 1) {
            free(freePath);
            return SAMCONF_CONFIG_NOT_FOUND;
        } else {
            ++pathCopy;
        }
    }

    if (pathCopy[strlen(pathCopy) - 1] == '/') {
        pathCopy[strlen(pathCopy) - 1] = '\0';
    }

    lastConfig = strrchr(pathCopy, '/');
    if (lastConfig) {
        ++lastConfig;
    } else {
        lastConfig = pathCopy;
    }

    char *token = strsep(&pathCopy, "/");

    while (token) {
        for (childItr = 0; childItr < childCount; childItr++) {
            node = parent->children[childItr];
            if (strcmp(node->key, token) == 0) {
                if (token == lastConfig) {
                    *result = node;
                    free(freePath);
                    return SAMCONF_CONFIG_OK;
                } else if (node->children) {
                    childItr = 0;
                    childCount = node->childCount;
                    parent = node;
                    break;
                } else {
                    free(freePath);
                    return SAMCONF_CONFIG_NOT_FOUND;
                }
            }
        }

        if ((childCount != 0) && (childItr == childCount)) {
            free(freePath);
            return SAMCONF_CONFIG_NOT_FOUND;
        }

        token = strsep(&pathCopy, "/");
    }

    free(freePath);
    return SAMCONF_CONFIG_NOT_FOUND;
}

samconfConfigStatusE_t samconfConfigGetString(const samconfConfig_t *root, const char *path, const char **result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (!node || (SAMCONF_CONFIG_OK != status)) {
        *result = NULL;
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_STRING) {
        *result = node->value.string;
    } else {
        *result = NULL;
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetString(samconfConfig_t *config, const char *stringValue) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL && stringValue != NULL) {
        config->value.string = strdup(stringValue);
        if (config->value.string != NULL) {
            config->type = SAMCONF_CONFIG_VALUE_STRING;
            status = SAMCONF_CONFIG_OK;
        }
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetBool(const samconfConfig_t *root, const char *path, bool *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_BOOLEAN) {
        *result = node->value.boolean;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetInt32(const samconfConfig_t *root, const char *path, int32_t *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_INT) {
        *result = node->value.integer;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetInt(samconfConfig_t *config, int64_t intValue) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.integer = intValue;
        config->type = SAMCONF_CONFIG_VALUE_INT;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetBool(samconfConfig_t *config, bool value) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.boolean = value;
        config->type = SAMCONF_CONFIG_VALUE_BOOLEAN;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetReal(samconfConfig_t *config, double value) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    if (config != NULL) {
        config->value.real = value;
        config->type = SAMCONF_CONFIG_VALUE_REAL;
        status = SAMCONF_CONFIG_OK;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigGetReal(const samconfConfig_t *root, const char *path, double *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);

    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_REAL) {
        *result = node->value.real;
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

samconfConfigStatusE_t samconfConfigSetValueFromString(samconfConfig_t *config, const char *value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char *endPtr = NULL;

    if (!value) {
        result = SAMCONF_CONFIG_ERROR;
    }

    if (result == SAMCONF_CONFIG_OK) {
        if (strcasecmp(value, "true") == 0) {
            config->type = SAMCONF_CONFIG_VALUE_BOOLEAN;
            config->value.boolean = true;
        } else if (strcasecmp(value, "false") == 0) {
            config->type = SAMCONF_CONFIG_VALUE_BOOLEAN;
            config->value.boolean = false;
        } else {
            config->value.integer = strtoll(value, &endPtr, 10);
            if (*endPtr == '\0') {
                config->type = SAMCONF_CONFIG_VALUE_INT;
            } else {
                config->value.real = strtod(value, &endPtr);
                if (*endPtr == '\0') {
                    config->type = SAMCONF_CONFIG_VALUE_REAL;
                } else {
                    config->value.string = strdup(value);
                    config->type = SAMCONF_CONFIG_VALUE_STRING;
                }
            }
        }
    }

    return result;
}

int samconfInitConfig() {
    return 0;
}

const char *samconfConfigGetStringOr(const samconfConfig_t *root, const char *path, const char *defaultValue) {
    const char *result = NULL;
    samconfConfigStatusE_t status = samconfConfigGetString(root, path, &result);
    if (status != SAMCONF_CONFIG_OK) {
        result = defaultValue;
    }
    return result;
}
bool samconfConfigGetBoolOr(const samconfConfig_t *root, const char *path, bool defaultValue) {
    bool result;
    samconfConfigStatusE_t status = samconfConfigGetBool(root, path, &result);
    if (status != SAMCONF_CONFIG_OK) {
        result = defaultValue;
    }
    return result;
}
int32_t samconfConfigGetInt32Or(const samconfConfig_t *root, const char *path, int32_t defaultValue) {
    int32_t result;
    samconfConfigStatusE_t status = samconfConfigGetInt32(root, path, &result);
    if (status != SAMCONF_CONFIG_OK) {
        result = defaultValue;
    }
    return result;
}
double samconfConfigGetRealOr(const samconfConfig_t *root, const char *path, double defaultValue) {
    double result;
    samconfConfigStatusE_t status = samconfConfigGetReal(root, path, &result);
    if (status != SAMCONF_CONFIG_OK) {
        result = defaultValue;
    }
    return result;
}
