// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

__BEGIN_DECLS

const struct samconfConfigBackendOps *samconfGetBackendOps(samconfBackendOptions_t idx);
samconfConfigStatusE_t samconfLookupBackend(const char *location, struct samconfConfigBackend **backend);
samconfConfigStatusE_t samconfVerifySignature(const char *location);
samconfConfigStatusE_t samconfLoad(const char *location, bool enforceSignature, samconfConfig_t **const config);

samconfConfigStatusE_t samconfConfigNew(samconfConfig_t **const config);
samconfConfigStatusE_t samconfConfigInit(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigDelete(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigAdd(samconfConfig_t *parent, samconfConfig_t *child);
samconfConfigStatusE_t samconfConfigDeleteMembers(samconfConfig_t *config);
samconfConfigStatusE_t samconfConfigGet(const samconfConfig_t *root, const char *path, const samconfConfig_t **result);
samconfConfigStatusE_t samconfConfigGetString(const samconfConfig_t *root, const char *path, const char **result);
samconfConfigStatusE_t samconfConfigSetString(samconfConfig_t *config, const char *stringValue);
samconfConfigStatusE_t samconfConfigGetBool(const samconfConfig_t *root, const char *path, bool *result);
samconfConfigStatusE_t samconfConfigGetInt32(const samconfConfig_t *root, const char *path, int32_t *result);
samconfConfigStatusE_t samconfConfigGetReal(const samconfConfig_t *root, const char *path, double *result);
samconfConfigStatusE_t samconfConfigSetInt(samconfConfig_t *config, int64_t intValue);
samconfConfigStatusE_t samconfConfigSetBool(samconfConfig_t *config, bool value);
samconfConfigStatusE_t samconfConfigSetReal(samconfConfig_t *config, double value);
int samconfInitConfig();

/**
 * Create a path to the config, upto the top most parent.
 *
 * Parameters:
 *     config(samconfConfig_t *): config from which path is to be created.
 *     path (const char*):  path from root parent to given config
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */
samconfConfigStatusE_t samconfGetParentPath(const samconfConfig_t *config, const char **path);

/**
 * Creates a samconfConfig_t struct by following the path parameter. The value is then set to
 * the last node in the path
 *
 * Parameters:
 *     root (samconfConfig_t *): config to which value is to be set.
 *     value (const char*):  value to be set. It can be uint64_t, bool, double or string
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */
samconfConfigStatusE_t samconfConfigSetValueFromString(samconfConfig_t *config, const char *value);

/*******************************************************************
 * Get a string from a samconfConfig_t.
 * If there is no string found under the provided search path
 * a default value is returned.
 * Also if any error is encountered while searching the configuration
 * the defaultValue is returned.
 *
 * Parameters:
 *      root: the configuration to look in
 *      path: the path where to look for the string
 *      defaultValue: the default value to return
 * Returns:
 *      - the string found in root under path
 *      - defaultValue if nothing was found or an error occurred
 ******************************************************************/
const char *samconfConfigGetStringOr(const samconfConfig_t *root, const char *path, const char *defaultValue);

/*******************************************************************
 * Get a bool from a samconfConfig_t.
 * If there is no string found under the provided search path
 * a default value is returned.
 * Also if any error is encountered while searching the configuration
 * the defaultValue is returned.
 *
 * Parameters:
 *      root: the configuration to look in
 *      path: the path where to look for the string
 *      defaultValue: the default value to return
 * Returns:
 *      - the bool found in the root under path
 *      - defaultValue if nothing was found or an error occurred
 ******************************************************************/
bool samconfConfigGetBoolOr(const samconfConfig_t *root, const char *path, bool defaultValue);

/*******************************************************************
 * Get a int32_t from a samconfConfig_t.
 * If there is no string found under the provided search path
 * a default value is returned.
 * Also if any error is encountered while searching the configuration
 * the defaultValue is returned.
 *
 * Parameters:
 *      root: the configuration to look in
 *      path: the path where to look for the string
 *      defaultValue: the default value to return
 * Returns:
 *      - the int32_t found in the root under path
 *      - defaultValue if nothing was found or an error occurred
 ******************************************************************/
int32_t samconfConfigGetInt32Or(const samconfConfig_t *root, const char *path, int32_t defaultValue);

/*******************************************************************
 * Get a double from a samconfConfig_t.
 * If there is no string found under the provided search path
 * a default value is returned.
 * Also if any error is encountered while searching the configuration
 * the defaultValue is returned.
 *
 * Parameters:
 *      root: the configuration to look in
 *      path: the path where to look for the string
 *      defaultValue: the default value to return
 * Returns:
 *      - the double found in the root under path
 *      - defaultValue if nothing was found or an error occurred
 ******************************************************************/
double samconfConfigGetRealOr(const samconfConfig_t *root, const char *path, double defaultValue);

__END_DECLS
