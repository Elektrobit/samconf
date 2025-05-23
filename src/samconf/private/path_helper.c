// SPDX-License-Identifier: MIT
#include "samconf/path_helper.h"

#include <safu/common.h>
#include <safu/defines.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/types.h>
#include <samconf/samconf_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int _pathGetSegmentCount(const char *path) {
    int segcount = 0;
    if (path != NULL) {
        size_t pathLen = strlen(path) + 1;  // to include '\0'
        for (size_t i = 1; i < pathLen; i++) {
            if ((path[i] == '/' || path[i] == '\0') && path[i - 1] != '/') {
                segcount++;
            }
        }
    }
    return segcount;
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

samconfConfigStatusE_t samconfPathCreateArray(const char *path, char ***patharray, int *count) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    int segmentCount = 0;
    char *pathCopy = NULL;
    char *freePath = NULL;
    *count = segmentCount;

    if (path != NULL) {
        freePath = pathCopy = strdup(path);
        if (freePath != NULL && pathCopy != NULL) {
            segmentCount = _pathGetSegmentCount(pathCopy);

            *patharray = safuAllocMem(NULL, segmentCount * sizeof(char *));

            if (*patharray != NULL) {
                char *token;
                int index = 0;
                bool arrayCreated = true;
                while ((token = strtok_r(pathCopy, "/", &pathCopy))) {
                    (*patharray)[index] = strdup(token);
                    if (!(*patharray)[index]) {
                        samconfPathDeleteArray(*patharray, index);
                        arrayCreated = false;
                        break;
                    }
                    index++;
                }
                if (arrayCreated) {
                    *count = segmentCount;
                    status = SAMCONF_CONFIG_OK;
                }
            }
        }
    }
    free(freePath);
    return status;
}

void samconfPathDeleteArray(char **patharray, int count) {
    for (int i = 0; i < count; i++) {
        free(patharray[i]);
    }
    free(patharray);
}

samconfConfigStatusE_t samconfPathGetPathUntil(char **patharray, int index, char **newPath) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    size_t pathLen = 0;

    for (int i = 0; i <= index; i++) {
        pathLen += strlen(patharray[i]);
    }

    pathLen += index + 1;

    *newPath = safuAllocMem(NULL, pathLen * sizeof(char));

    if (*newPath != NULL) {
        *newPath[0] = '\0';

        for (int i = 0; i <= index; i++) {
            strcat(*newPath, patharray[i]);
            if (i != index) {
                strcat(*newPath, "/");
            }
        }

        status = SAMCONF_CONFIG_OK;
    }
    return status;
}

samconfConfigStatusE_t samconfPathGetPath(const samconfConfig_t *config, const char **path) {
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
