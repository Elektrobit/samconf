// SPDX-License-Identifier: MIT
#include "samconf/env_backend.h"

#include <errno.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <libgen.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"
#include "samconf/uri.h"

samconfConfigStatusE_t samconfEnvBackendOpen(const char *location, samconfConfigBackend_t *backend) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfUri_t *uri;
    const char *envScheme = "env";

    result = samconfUriNew(&uri, location);
    if (result == SAMCONF_CONFIG_OK) {
        if (strcasecmp(uri->scheme, envScheme) == 0) {
            backend->backendHandle = uri;
        } else {
            result = samconfUriDelete(uri);
            if (result != SAMCONF_CONFIG_OK) {
                safuLogErr("Config Uri deletion failed");
            }
            safuLogCritF("Invalid location for env variable configuration: %s.", location);

            result = SAMCONF_CONFIG_ERROR;
        }
    } else {
        safuLogCritF("URI creation failed for location : %s.", location);
    }
    return result;
}

samconfConfigStatusE_t samconfEnvBackendSupports(const char *location, bool *isSupported) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    char *protocol = strndup(location, 3);

    if (protocol != NULL) {
        if (strcasecmp(protocol, "env") == 0) {
            *isSupported = true;
            result = SAMCONF_CONFIG_OK;
        }
        free(protocol);
    }
    return result;
}

samconfConfigStatusE_t samconfEnvBackendClose(samconfConfigBackend_t *backend) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    if (backend != NULL) {
        result = samconfUriDelete(backend->backendHandle);
        if (result != SAMCONF_CONFIG_OK) {
            safuLogErr("Config Uri deletion failed");
        }
        free(backend->originalHandle);
    }
    return result;
}

static void _replaceCharsInString(char *str, uint32_t len, char old, char new) {
    uint32_t i = 0;
    while (str[i] != '\0' && i < len) {
        if (str[i] == old) str[i] = new;
        i++;
    }
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

samconfConfigStatusE_t samconfEnvBackendCreateConfig(samconfConfig_t **root, const char *path, const char *value) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char *pathCopy = NULL;
    char *freePath = NULL;
    char *lastConfig = NULL;
    char *pathToToken = NULL;
    samconfConfig_t *node = NULL;
    samconfConfig_t *parent = NULL;
    const samconfConfig_t *existingNode = NULL;

    if (root == NULL || path == NULL) {
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
                        node = (samconfConfig_t *)existingNode;
                        if (strcasecmp(pathToToken, token) == 0) {
                            if (token == lastConfig) {
                                result = samconfConfigSetValueFromString(node, value);
                                if (result != SAMCONF_CONFIG_OK) {
                                    safuLogWarnF(
                                        "Invalid type or null passed as value, node : %s will be set to default",
                                        node->key);
                                }
                            }
                            parent = node;
                        } else {
                            parent = node;
                        }
                    } else {
                        result = samconfConfigNew(&node);
                        if (result != SAMCONF_CONFIG_OK) {
                            safuLogErr("samconfConfigNew failed");
                            break;
                        }
                        node->key = strdup(token);
                        node->type = SAMCONF_CONFIG_VALUE_OBJECT;
                        if (token == lastConfig) {
                            result = samconfConfigSetValueFromString(node, value);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogWarnF("Invalid type or null passed as value, node : %s will be set to default",
                                             node->key);
                            }

                            result = samconfConfigAdd(parent, node);
                            if (result != SAMCONF_CONFIG_OK) {
                                safuLogErrF("samconfConfigAdd failed to add node : %s", node->key);
                                samconfConfigDelete(node);
                                break;
                            }
                        } else {
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

static samconfConfigStatusE_t _transform_env_to_config(bool isSigned, samconfConfig_t *config) {
    extern char **environ;
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;

    char *key = NULL;
    char *val = NULL;
    char *envCopy = NULL;
    size_t pos = 0;
    size_t len = 0;

    config->isSigned = isSigned;

    for (size_t i = 0; environ[i] != NULL; i++) {
        envCopy = strdup(environ[i]);
        if (envCopy != NULL) {
            pos = strcspn(envCopy, "=");
            key = envCopy;
            val = envCopy + pos + 1;
            key[pos] = '\0';
            len = strlen(key);
            if (len > 1) {
                _replaceCharsInString(key, strlen(key), '_', '/');
            }
            result = samconfEnvBackendCreateConfig(&config, key, val);
            if (result != SAMCONF_CONFIG_OK) {
                safuLogErrF("Config creation failed for given key : %s, value: %s\n", key, val);
                free(envCopy);
                break;
            }
            free(envCopy);
        }
    }

    return result;
}

samconfConfigStatusE_t samconfEnvBackendLoad(UNUSED samconfConfigBackend_t *backend, bool isSigned,
                                             samconfConfig_t **config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    result = samconfConfigNew(config);

    if (result == SAMCONF_CONFIG_OK) {
        (*config)->type = SAMCONF_CONFIG_VALUE_OBJECT;
        (*config)->key = strdup("root");

        result = _transform_env_to_config(isSigned, *config);
    } else {
        safuLogErr("Failed to initialize Config");
    }

    return result;
}

const samconfConfigBackendOps_t samconfEnvOps = {
    .supports = samconfEnvBackendSupports,
    .open = samconfEnvBackendOpen,
    .close = samconfEnvBackendClose,
    .load = samconfEnvBackendLoad,
};
