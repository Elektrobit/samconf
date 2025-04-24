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

/*******************************************************************
 * Creates a samconfConfig_t struct by following the path parameter. The value is then set to
 * the last node in the path
 *
 * Parameters:
 *     root : config to which value is to be set.
 *     value : value to be set. It can be uint64_t, bool, double or string
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
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
 *
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
 *
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
 *
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
 *
 * Returns:
 *      - the double found in the root under path
 *      - defaultValue if nothing was found or an error occurred
 ******************************************************************/
double samconfConfigGetRealOr(const samconfConfig_t *root, const char *path, double defaultValue);

/*******************************************************************
 * Inserts given samconfConfig_t struct by following the path parameter. The config is then added to
 * the last node in the path. If the path points to an existing node, then the config value is copied
 * to the exiting node.
 *
 * Parameters:
 *     root : root config to which further nodes from path are added.
 *     path : path followed to create the config
 *     config : config to be added as new node or copied to an existing node
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfInsertAt(samconfConfig_t **root, const char *path, samconfConfig_t *config);

/*******************************************************************
 * Creates a samconfConfig_t struct and sets the type to int and value to given integer value.
 * The struct is then inserted at the given path.
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path : path followed to create the config
 *     value : integer value of to be set to config
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCreateIntAt(samconfConfig_t **root, const char *path, int64_t value);

/*******************************************************************
 * Creates a samconfConfig_t struct and sets the type to double and value to given double value
 * and inserts it in the path.
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path : path followed to create the config
 *     value : double value of to be set to config
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCreateRealAt(samconfConfig_t **root, const char *path, double value);

/*******************************************************************
 * Creates a samconfConfig_t struct and sets the type to bool and value to given boolean value
 * and inserts it in the path.
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path : path followed to create the config
 *     value : boolean value of to be set to config
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCreateBoolAt(samconfConfig_t **root, const char *path, bool value);

/*******************************************************************
 * Creates a samconfConfig_t struct and extracts the right type and value from the string passed
 *
 * Eg:
 *     '42' -> type = int64_t, value = 42
 *     'true' -> type = bool, value = true
 *     '1.986' -> type = double, value = 1.986
 *     Anything else as string
 *
 * The extracted type and value is set to the config accordingly
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path : path followed to create the config
 *     value : string to be converted into appropriate value
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCreateFromStringAt(samconfConfig_t **root, const char *path, char *value);

/*******************************************************************
 * Creates a samconfConfig_t struct and sets the type to string and value to given string value
 * and inserts it in the path.
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path :  path followed to create the config
 *     value : string value of to be set to config
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCreateStringAt(samconfConfig_t **root, const char *path, const char *value);

/*******************************************************************
 * A _Generic macro to create a config with the given value. The values can be int64_t,
 * double, boolean or string.
 *
 * Parameters:
 *     root : root config to which further nodes from path a added.
 *     path :  path followed to create the config
 *     value : value of to be set to config of type int64_t, double, bool, char*, const char*
 *
 * Note:
 *     When value is of type char* , it means, it is the string representation of the value
 *     and the value can be any of the other supported types given above.
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
// clang-format off
// Rationale: Used version of clang-format does not format _Generic macros correctly. This is a known bug and has been
// fixed very recently. We may remove this exemption once we are on the new clang version as standard.
// See: https://github.com/llvm/llvm-project/issues/18080
#define samconfCreateAt(A, B, C)             \
    _Generic((C),                            \
        int64_t: samconfCreateIntAt,         \
        double: samconfCreateRealAt,         \
        bool: samconfCreateBoolAt,           \
        char *: samconfCreateFromStringAt,   \
        const char *: samconfCreateStringAt, \
        default: samconfCreateStringAt)(A, B, C)
// clang-format on

/*******************************************************************
 * Copies the type and value from the `from` config to the `to` config
 *
 * Parameters:
 *     from : config from which value and type are copied
 *     to : config to which value and type are copied to.
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfCopyConfigValue(const samconfConfig_t *from, samconfConfig_t *to);

/*******************************************************************
 * Merge the given configuration to the given root configuration.
 *
 * Parameters:
 *     mergedConfig : base config into which new config is merged
 *     configToMerge : config to be merged into root config
 *
 * Returns:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 ******************************************************************/
samconfConfigStatusE_t samconfMergeConfig(samconfConfig_t *mergedConfig, samconfConfig_t *configToMerge);

__END_DECLS
