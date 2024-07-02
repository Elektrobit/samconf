// SPDX-License-Identifier: MIT
#pragma once

#include <json-c/json.h>
#include <samconf/samconf.h>

/********************************************************************************
 * Attach the contents of a config from a `json_object` to a given
 * samconfConfig_t node. Use this to create config mockups or pieces of a config
 * tree for unit tests. The given config node shall be freed by
 * `samconfConfigDeleteMemebers`.
 *
 * Parameters:
 *      jobj: the json object containing the configuration snippet
 *      isSigned: set to true or false to mark the content to be from an signed
 *                source or not
 *      config: pointer to a valid config node used as root node for the new
 *              config
 * Returns:
 *      - SAMCONF_CONFIG_OK if config was created successfully
 *      - SAMCONF_CONFIG_PARSER_ERROR if the given config snippet could not be parsed
 *      - SAMCONF_CONFIG_ERROR on any other error
 ********************************************************************************/
samconfConfigStatusE_t samconfUtilCreateMockConfig(json_object *jobj, bool isSigned, samconfConfig_t *config);

/********************************************************************************
 * Attach the contents of a config from a json string to a given samconfConfig_t
 * node. Use this to create config mockups or pieces of a config tree for unit
 * tests. The given config node shall be freed by `samconfConfigDeleteMemebers`.
 *
 * Parameters:
 *      jstr: the json string containing the configuration snippet
 *      isSigned: set to true or false to mark the content to be from an signed
 *                source or not
 *      config: pointer to a valid config node used as root node for the new
 *              config
 * Returns:
 *      - SAMCONF_CONFIG_OK if config was created successfully
 *      - SAMCONF_CONFIG_PARSER_ERROR if the given config snippet could not be parsed
 *      - SAMCONF_CONFIG_ERROR on any other error
 ********************************************************************************/
samconfConfigStatusE_t samconfUtilCreateMockConfigFromStr(const char *jstr, bool isSigned, samconfConfig_t *config);
