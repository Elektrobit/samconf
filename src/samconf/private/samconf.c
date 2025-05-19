// SPDX-License-Identifier: MIT
#include "samconf/samconf.h"

#include <dirent.h>
#include <libgen.h>
#include <safu/common.h>
#include <safu/defines.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

#include "samconf/config_backend.h"
#include "samconf/crypto_utils.h"
#include "samconf/dummy_backend.h"
#include "samconf/env_backend.h"
#include "samconf/json_backend.h"
#include "samconf/path_helper.h"
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

static int _cmp(const void *left, const void *right) {
    return -strncmp(*(const char **)right, *(const char **)left, 256);
}

static samconfConfigStatusE_t _extendWithSinglePath(const char *configLocation, bool enforceSignature,
                                                    samconfConfig_t **conf) {
    samconfConfig_t *addConf = NULL;
    samconfConfigStatusE_t res = samconfLoad(configLocation, enforceSignature, &addConf);
    if (res != SAMCONF_CONFIG_OK) {
        safuLogWarnF("Failed to load \"%s\" (%d)", configLocation, res);
    } else {
        res = samconfConfigMergeConfig(conf, addConf);
        if (res == SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED) {
            safuLogDebugF("not allowed to merge in \"%s\"", configLocation);
        } else if (res != SAMCONF_CONFIG_OK) {
            safuLogWarnF("samconfMergeConfig() merge %d failed (%d)", res, res);
        }
        samconfConfigStatusE_t tmpRes = samconfConfigDelete(addConf);
        if (tmpRes != SAMCONF_CONFIG_OK) {
            safuLogWarnF("coudn't delete the tmp config: %d", tmpRes);
        }
    }
    return res;
}

static samconfConfigStatusE_t _extendWithDirectory(DIR *confDir, bool enforceSignature, const char *const path,
                                                   samconfConfig_t **conf) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_NOT_FOUND;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    safuVec_t confList;
    int vecRes = safuVecCreate(&confList, 100, sizeof(char *));
    if (vecRes != 0) {
        status = SAMCONF_CONFIG_ERROR;
        safuLogErr("Failed to create config list");
    }
    if (status == SAMCONF_CONFIG_OK) {
        struct dirent *ep;
        while ((ep = readdir(confDir))) {
            if (ep->d_type == DT_REG) {
                char *entry = strndup(ep->d_name, 255);
                if (entry == NULL) {
                    status = SAMCONF_CONFIG_ERROR;
                    safuLogErr("Failed to copy config entry");
                    break;
                }
                vecRes = safuVecPush(&confList, &entry);
                if (vecRes != 0) {
                    status = SAMCONF_CONFIG_ERROR;
                    safuLogErr("Failed to add to config list");
                    break;
                }
            }
        }
    }
    if (status != SAMCONF_CONFIG_OK) {
        result = status;
    } else {
        qsort(confList.data, confList.elementCount, confList.elementSize, _cmp);
        size_t locationLen = strlen(path);
        for (size_t i = 0; i < safuVecElements(&confList); i++) {
            char *configFile = *(char **)safuVecGet(&confList, i);
            char *configLocation = malloc(strlen(configFile) + locationLen + 2);
            sprintf(configLocation, "%s/%s", path, configFile);
            samconfConfigStatusE_t res = _extendWithSinglePath(configLocation, enforceSignature, conf);
            if (res == SAMCONF_CONFIG_INVALID_SIGNATURE) {
                result = SAMCONF_CONFIG_INVALID_SIGNATURE;
            } else if (res == SAMCONF_CONFIG_OK && result != SAMCONF_CONFIG_INVALID_SIGNATURE) {
                result = SAMCONF_CONFIG_OK;
            } else if (res == SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED && result != SAMCONF_CONFIG_OK &&
                       result != SAMCONF_CONFIG_INVALID_SIGNATURE) {
                safuLogDebugF("not allowed to merge %s", configLocation);
                result = SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED;
            } else if (res == SAMCONF_CONFIG_SIGNATURE_NOT_FOUND && result != SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED &&
                       result != SAMCONF_CONFIG_OK && result != SAMCONF_CONFIG_INVALID_SIGNATURE) {
                result = SAMCONF_CONFIG_SIGNATURE_NOT_FOUND;
            }
            free(configLocation);
        }
        for (size_t i = 0; i < safuVecElements(&confList); i++) {
            char *configFile = *(char **)safuVecGet(&confList, i);
            free(configFile);
        }
    }
    safuVecFree(&confList);
    return result;
}

static samconfConfigStatusE_t _extendWithLocation(const samconfConfigLocation_t *location,
                                                  samconfConfig_t **const conf) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_NOT_FOUND;
    DIR *confDir = opendir(location->path);
    if (confDir != NULL) {
        result = _extendWithDirectory(confDir, location->enforceSignature, location->path, conf);
        if (result == SAMCONF_CONFIG_NOT_FOUND) {
            safuLogWarnF("No config found in \"%s\"", location->path);
        } else if (result == SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED) {
            safuLogWarnF("merge rules don't allow anything in \"%s\" to be merged", location->path);
        } else if (result != SAMCONF_CONFIG_OK) {
            safuLogErrF("couldn't extend with \"%s\" (%d)", location->path, result);
        }
        closedir(confDir);
    } else {
        result = _extendWithSinglePath(location->path, location->enforceSignature, conf);
        if (result == SAMCONF_CONFIG_NOT_FOUND) {
            safuLogWarnF("no config could be found at \"%s\"", location->path);
        } else if (result == SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED) {
            safuLogWarnF("extending with \"%s\" not allowed by merge rules", location->path);
        } else if (result != SAMCONF_CONFIG_OK) {
            safuLogErrF("couldn't extend with \"%s\"", location->path);
        }
    }
    return result;
}

samconfConfigStatusE_t samconfLoadAndMerge(const samconfConfigLocation_t locations[], size_t locationsSize,
                                           samconfConfig_t **const config) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    if (locations == NULL) {
        safuLogErr("Invalid Call to samconfLoad, missing config locations");
        return SAMCONF_CONFIG_ERROR;
    }
    if (config == NULL) {
        safuLogErr("Invalid Call to samconfLoad, uninitialized configuration");
        return SAMCONF_CONFIG_ERROR;
    }
    if (*config == NULL) {
        safuLogDebug("Creating new config");
        status = samconfConfigNew(config);
        if (status == SAMCONF_CONFIG_OK) {
            (*config)->type = SAMCONF_CONFIG_VALUE_OBJECT;
        }
    }
    if (status != SAMCONF_CONFIG_OK) {
        return status;
    }
    status = SAMCONF_CONFIG_NOT_FOUND;
    samconfConfigStatusE_t tmpRes = SAMCONF_CONFIG_OK;
    for (size_t i = 0; i < locationsSize; i++) {
        switch (locations[i].type) {
            case SAMCONF_CONFIG_LOCATION_TYPE_CONFIG:
                if (locations[i].path == NULL) {
                    continue;
                }
                tmpRes = samconfConfigMergeConfig(config, locations[i].config);
                break;
            case SAMCONF_CONFIG_LOCATION_TYPE_PATH:
                tmpRes = _extendWithLocation(&locations[i], config);
                break;
            default:
                safuLogWarn("not a valid config location");
                continue;
        }
        if (tmpRes == SAMCONF_CONFIG_OK) {
            status = SAMCONF_CONFIG_OK;
        } else if (tmpRes == SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED && status == SAMCONF_CONFIG_OK) {
            status = SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED;
        } else {
            safuLogWarnF("some error extending With location \"%s\"", locations[i].path);
        }
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

samconfConfigStatusE_t samconfCopyConfigValue(const samconfConfig_t *from, samconfConfig_t *to) {
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
            case SAMCONF_CONFIG_VALUE_NONE:
            case SAMCONF_CONFIG_VALUE_ARRAY:
            case SAMCONF_CONFIG_VALUE_OBJECT:
                result = SAMCONF_CONFIG_OK;
                break;
            default:
                result = samconfConfigSetString(to, from->value.string);
        }
    }

    return result;
}

samconfConfigStatusE_t samconfConfigCopyConfig(const samconfConfig_t *from, samconfConfig_t *to) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    if (from == NULL || to == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        to->key = strdup(from->key);

        if (to->key) {
            to->isSigned = from->isSigned;
            to->type = from->type;

            result = samconfCopyConfigValue(from, to);
        }
    }
    return result;
}

static samconfConfigStatusE_t _findNextConfigAtLevel(const samconfConfig_t *root, const samconfConfig_t *configToFind,
                                                     bool *found, const samconfConfig_t **nextConfig) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_NOT_FOUND;
    if (root == NULL || *nextConfig) {
        result = SAMCONF_CONFIG_ERROR;
    }

    if (result == SAMCONF_CONFIG_NOT_FOUND) {
        if (*found && *nextConfig == NULL) {
            *nextConfig = root;
            result = SAMCONF_CONFIG_OK;
        } else if (root == configToFind) {
            *found = true;
        }
        if (result == SAMCONF_CONFIG_NOT_FOUND) {
            if (root->children != NULL && root->childCount != 0) {
                for (size_t i = 0; i < root->childCount; i++) {
                    result = _findNextConfigAtLevel(root->children[i], configToFind, found, nextConfig);
                }
            }
        }
    }

    return result;
}

samconfConfigStatusE_t samconfConfigNext(const samconfConfig_t *root, const samconfConfig_t *configToFind,
                                         const samconfConfig_t **nextConfig) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    bool found = false;
    if (root != NULL && configToFind != NULL) {
        result = _findNextConfigAtLevel(root, configToFind, &found, nextConfig);
    }
    return result;
}

samconfConfigStatusE_t samconfInsertAt(samconfConfig_t **root, const char *path, samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char **patharray = NULL;
    char *pathToToken = NULL;
    int segmentCount = 0;
    samconfConfig_t *node = NULL;
    samconfConfig_t *parent = NULL;
    const samconfConfig_t *existingNode = NULL;

    if (root == NULL || path == NULL || config == NULL) {
        result = SAMCONF_CONFIG_ERROR;
    }

    if (result == SAMCONF_CONFIG_OK) {
        result = samconfPathCreateArray(path, &patharray, &segmentCount);
        if (patharray == NULL || segmentCount == 0) {
            result = SAMCONF_CONFIG_ERROR;
        }

        if (result == SAMCONF_CONFIG_OK) {
            parent = *root;
            for (int i = 0; i < segmentCount; i++) {
                result = samconfPathGetPathUntil(patharray, i, &pathToToken);
                if (result == SAMCONF_CONFIG_OK) {
                    result = samconfConfigGet(*root, pathToToken, &existingNode);
                    if (result == SAMCONF_CONFIG_OK) {
                        if (i == segmentCount - 1) {
                            safuLogErrF("cannot insert node : %s, already exists", patharray[i]);
                            free(pathToToken);
                            break;
                        } else {
                            parent = (samconfConfig_t *)existingNode;
                        }
                    } else if (result == SAMCONF_CONFIG_ERROR) {
                        safuLogErrF("error getting node : %s", patharray[i]);
                        free(pathToToken);
                        break;
                    } else {
                        if (i == segmentCount - 1) {
                            node = config;
                        } else {
                            result = samconfConfigNew(&node);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogErr("samconfConfigNew failed");
                                free(pathToToken);
                                break;
                            }
                            node->key = strdup(patharray[i]);
                            node->type = SAMCONF_CONFIG_VALUE_OBJECT;
                        }
                        result = samconfConfigAdd(parent, node);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErrF("samconfConfigAdd failed to add node : %s", config->key);
                            free(pathToToken);
                            break;
                        }
                        parent = node;
                    }
                    free(pathToToken);
                }
            }
            samconfPathDeleteArray(patharray, segmentCount);
        }
    }

    return result;
}

samconfConfigStatusE_t samconfConfigGet(const samconfConfig_t *root, const char *path, const samconfConfig_t **result) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    char **patharray = NULL;
    int segmentCount = 0;
    size_t childItr = 0;
    size_t childCount = 0;

    if (!root || !path || !result) {
        status = SAMCONF_CONFIG_ERROR;
    } else if (!root->children || root->childCount == 0) {
        status = SAMCONF_CONFIG_NOT_FOUND;
    } else {
        const samconfConfig_t *node = *(root->children);
        const samconfConfig_t *parent = root;
        childCount = root->childCount;
        status = samconfPathCreateArray(path, &patharray, &segmentCount);
        if (status == SAMCONF_CONFIG_OK) {
            for (int i = 0; i < segmentCount; i++) {
                for (childItr = 0; childItr < childCount; childItr++) {
                    if (parent->children) {
                        node = parent->children[childItr];
                        if (strcmp(node->key, patharray[i]) == 0) {
                            if (i == segmentCount - 1) {
                                *result = node;
                                status = SAMCONF_CONFIG_OK;
                                break;
                            } else if (node->children) {
                                childItr = 0;
                                childCount = node->childCount;
                                parent = node;
                                break;
                            } else {
                                if (i == segmentCount - 1) {
                                    status = SAMCONF_CONFIG_NOT_FOUND;
                                    break;
                                } else {
                                    parent = node;
                                    break;
                                }
                            }
                        }
                    } else {
                        status = SAMCONF_CONFIG_NOT_FOUND;
                        break;
                    }
                }

                if ((childCount != 0) && (childItr == childCount)) {
                    status = SAMCONF_CONFIG_NOT_FOUND;
                    break;
                }
            }
            samconfPathDeleteArray(patharray, segmentCount);
        }
    }

    return status;
}

static bool _followsMergeRule(samconfConfig_t *mergedConfig, samconfConfig_t *configToMerge) {
    return ((mergedConfig->isSigned && configToMerge->isSigned) ||
            (!mergedConfig->isSigned && !configToMerge->isSigned) ||
            (!mergedConfig->isSigned && configToMerge->isSigned));
}

samconfConfigStatusE_t samconfConfigMergeConfig(samconfConfig_t **mergedConfig, samconfConfig_t *configToMerge) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    if (configToMerge != NULL) {
        if (_followsMergeRule(*mergedConfig, configToMerge)) {
            samconfConfig_t *root = configToMerge;
            const samconfConfig_t *configToFind = root;
            const samconfConfig_t *nextConfig = NULL;
            do {
                samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
                status = samconfConfigNext(root, configToFind, &nextConfig);
                if (nextConfig != NULL) {
                    samconfConfig_t *node = NULL;
                    status = samconfConfigNew(&node);
                    if (status != SAMCONF_CONFIG_OK) {
                        safuLogErr("Creating new node to merge failed");
                        break;
                    }

                    status = samconfConfigCopyConfig(nextConfig, node);
                    if (status != SAMCONF_CONFIG_OK) {
                        safuLogErr("Copying to new node failed");
                        result = samconfConfigDelete(node);
                        break;
                    }

                    char *path = NULL;
                    status = samconfPathGetPath(nextConfig, (const char **)&path);
                    if (status != SAMCONF_CONFIG_OK) {
                        safuLogErr("Fetching path to node in config failed");
                        result = samconfConfigDelete(node);
                        break;
                    }

                    samconfConfig_t *nodeInMerge = NULL;
                    status = samconfConfigGet(*mergedConfig, path, (const samconfConfig_t **)&nodeInMerge);
                    if (status == SAMCONF_CONFIG_NOT_FOUND) {
                        status = samconfInsertAt(mergedConfig, path, node);
                        if (status != SAMCONF_CONFIG_OK) {
                            safuLogErr("Inserting node to merge config failed");
                            result = samconfConfigDelete(node);
                            free(path);
                            break;
                        }
                    } else if (status == SAMCONF_CONFIG_OK) {
                        safuLogInfo("Node found");
                        status = samconfCopyConfigValue(node, nodeInMerge);
                        if (status != SAMCONF_CONFIG_OK) {
                            safuLogErr("Overwriting exiting node failed");
                            result = samconfConfigDelete(node);
                            free(path);
                            break;
                        }
                        status = samconfConfigDelete(node);
                    } else {
                        safuLogErr("Search error : invalid node");
                        result = samconfConfigDelete(node);
                        free(path);
                        break;
                    }

                    if (status == SAMCONF_CONFIG_OK) {
                        configToFind = nextConfig;
                        nextConfig = NULL;
                    } else {
                        safuLogErr("Node found, but new node deletion failed");
                        free(path);
                        break;
                    }

                    free(path);
                } else {
                    configToFind = NULL;
                    if (!(*mergedConfig)->isSigned && configToMerge->isSigned) {
                        (*mergedConfig)->isSigned = configToMerge->isSigned;
                    }
                    result = SAMCONF_CONFIG_OK;
                }
            } while (configToFind != NULL);
        } else {
            result = SAMCONF_CONFIG_OVERWRITE_NOT_ALLOWED;
        }
    }

    return result;
}

samconfConfigStatusE_t samconfConfigMergeConfigs(samconfConfig_t **mergedConfig, samconfConfig_t **configsToMerge,
                                                 size_t configsCount) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    if (mergedConfig == NULL) {
        result = samconfConfigNew(mergedConfig);
        if (result == SAMCONF_CONFIG_OK) {
            (*mergedConfig)->type = SAMCONF_CONFIG_VALUE_OBJECT;
            (*mergedConfig)->key = strdup("root");
        }
    }

    for (size_t i = 0; i < configsCount; i++) {
        result = samconfConfigMergeConfig(mergedConfig, configsToMerge[i]);
        if (_followsMergeRule(*mergedConfig, configsToMerge[i])) {
            if (result != SAMCONF_CONFIG_OK) {
                break;
            }
        }
    }

    return result;
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
        if (config->type == SAMCONF_CONFIG_VALUE_STRING) {
            free(config->value.string);
        }
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

static void _level(const safuVec_t *const indicator) {
    bool *ind = indicator->data;
    for (size_t i = 0; i < indicator->elementCount; i++) {
        if (ind[i]) {
            printf("    ");
        } else {
            printf("│   ");
        }
    }
}

#define BOOL_STRING(b) (b) ? "true" : "false"
#define SIGNED(sgnd)   (sgnd) ? "✅" : "❌"

void _dumpConfigTree(const samconfConfig_t *const config, safuVec_t *indicator, bool last) {
    _level(indicator);
    if (last) {
        printf("└── %s %s", SIGNED(config->isSigned), config->key);
    } else {
        printf("├── %s %s", SIGNED(config->isSigned), config->key);
    }
    safuVecPush(indicator, &last);
    switch (config->type) {
        case SAMCONF_CONFIG_VALUE_OBJECT:
            printf("\n");
            for (size_t i = 0; i < config->childCount; i++) {
                last = i + 1 >= config->childCount;
                _dumpConfigTree(config->children[i], indicator, last);
            }
            break;
        case SAMCONF_CONFIG_VALUE_ARRAY:
            printf("\n");
            for (size_t i = 0; i < config->childCount; i++) {
                last = i + 1 >= config->childCount;
                _dumpConfigTree(config->children[i], indicator, last);
            }
            break;
        case SAMCONF_CONFIG_VALUE_BOOLEAN:
            printf(": %s\n", BOOL_STRING(config->value.boolean));
            break;
        case SAMCONF_CONFIG_VALUE_INT:
            printf(": %ld\n", config->value.integer);
            break;
        case SAMCONF_CONFIG_VALUE_REAL:
            printf(": %f\n", config->value.real);
            break;
        case SAMCONF_CONFIG_VALUE_STRING:
            printf(": \"%s\"\n", config->value.string);
            break;
        default:
            printf(": UNKNOWN/ERROR\n");
    }
    safuVecPop(indicator);
}

void samconfDumpConfigTree(const samconfConfig_t *const config) {
    if (config == NULL) {
        safuLogErr("Conifg to dump is NULL");
        return;
    }
    safuVec_t indicator;
    safuResultE_t res = safuVecCreate(&indicator, 10, sizeof(bool));
    if (res != SAFU_RESULT_OK) {
        safuLogErr("Failed to create indent vec\n");
        return;
    }
    printf("%s / (%s)\n", SIGNED(config->isSigned), config->key);
    for (size_t i = 0; i < config->childCount; i++) {
        bool last = i + 1 >= config->childCount;
        _dumpConfigTree(config->children[i], &indicator, last);
    }
    res = safuVecFree(&indicator);
    if (res != SAFU_RESULT_OK) {
        safuLogErr("Failed to free indent vec");
    }
}
