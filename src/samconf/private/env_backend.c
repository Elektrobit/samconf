// SPDX-License-Identifier: MIT
#include "samconf/env_backend.h"

#include <errno.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
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

static void _replaceCharsInString(char *str, size_t len, char old, char new) {
    size_t i = 0;
    while (str[i] != '\0' && i < len) {
        if (str[i] == old) str[i] = new;
        i++;
    }
}

static samconfConfigStatusE_t _transform_env_to_config(samconfUri_t *uri, bool isSigned, samconfConfig_t *config) {
    extern char **environ;
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    char *rootKey = "root";
    char *key = NULL;
    char *newKey = NULL;
    char *val = NULL;
    char *envCopy = NULL;
    size_t pos = 0;
    size_t len = 0;
    char *param = "envPrefix";
    char *filter = NULL;

    config->isSigned = isSigned;

    result = samconfUriGetParameter(uri, param, &filter);

    if (result != SAMCONF_CONFIG_OK || filter == NULL) {
        safuLogErrF("Invalid filter string in URI : %s", uri->plain);
    } else {
        for (size_t i = 0; environ[i] != NULL; i++) {
            envCopy = strdup(environ[i]);
            if (envCopy != NULL) {
                pos = strcspn(envCopy, "=");
                key = envCopy;
                val = envCopy + pos + 1;
                key[pos] = '\0';
                if (strstr(envCopy, filter) != NULL) {
                    len = strlen(key);
                    if (len > 1) {
                        _replaceCharsInString(key, len, '_', '/');
                    }
                    len += strlen(rootKey);
                    newKey = safuAllocMem(NULL, len + 2);
                    if (newKey == NULL) {
                        safuLogErr("memory allocation for key and root key failed");
                        free(envCopy);
                        break;
                    }
                    int ret = snprintf(newKey, len + 2, "%s/%s", rootKey, key);
                    if (ret < 0) {
                        safuLogErr("adding root to key failed");
                        free(newKey);
                        free(envCopy);
                        break;
                    }

                    result = samconfCreateAt(&config, key, val);
                    if (result != SAMCONF_CONFIG_OK) {
                        safuLogErrF("Config creation failed for given key : %s, value: %s\n", key, val);
                        free(newKey);
                        free(envCopy);
                        break;
                    }
                    free(newKey);
                }
                free(envCopy);
            }
        }
        free(filter);
    }

    return result;
}

samconfConfigStatusE_t samconfEnvBackendLoad(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;

    if (backend == NULL) {
        safuLogErr("Failed to create config tree due to invalid backend");
        result = SAMCONF_CONFIG_ERROR;
    }

    if (result == SAMCONF_CONFIG_OK) {
        result = samconfConfigNew(config);

        if (result == SAMCONF_CONFIG_OK) {
            (*config)->type = SAMCONF_CONFIG_VALUE_OBJECT;
            (*config)->key = strdup("root");
            if ((*config)->key == NULL) {
                safuLogErr("strdup failed to set root key");
                result = SAMCONF_CONFIG_ERROR;
            } else {
                result = _transform_env_to_config(backend->backendHandle, isSigned, *config);
            }
        } else {
            safuLogErr("Failed to initialize Config");
        }
    }

    return result;
}

const samconfConfigBackendOps_t samconfEnvOps = {
    .supports = samconfEnvBackendSupports,
    .open = samconfEnvBackendOpen,
    .close = samconfEnvBackendClose,
    .load = samconfEnvBackendLoad,
};
