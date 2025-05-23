// SPDX-License-Identifier: MIT
#pragma once

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

extern const samconfConfigBackendOps_t samconfEnvOps;

__BEGIN_DECLS

/**
 * Opens an Env backend, creating an URI from the location string passed
 * and adds the created URI to the backendHandle member of the backend parameter.
 *
 * Parameters:
 *     location (const char*):  environment variable location string (env://)
 *     backend (samconfConfigBackend_t *): Configuration backend for environment variables
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */

samconfConfigStatusE_t samconfEnvBackendOpen(const char *location, samconfConfigBackend_t *backend);

/**
 * Loads a configuration by creating a samconfConfig_t struct by following the path parameter and sets flag to
 * to indicate if config is signed or not.
 *
 * Parameters:
 *     backend (samconfConfigBackend_t *): Configuration backend for environment variables
 *     isSigned (bool): is the given configuration file signed or not
 *     root (samconfConfig_t *): root config to which further nodes from path a added.
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */
samconfConfigStatusE_t samconfEnvBackendLoad(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);

/**
 * Check if the given backend for env variables is supported or not.
 *
 * Parameters:
 *     location (const char*):  environment variable location string (env://)
 *     isSupported (bool): is the given backend supported or not
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */
samconfConfigStatusE_t samconfEnvBackendSupports(const char *location, bool *isSupported);

/**
 * Close given backend by deleting its members
 *
 * Parameters:
 *     backend (samconfConfigBackend_t *): Configuration backend for environment variables
 *
 * Returns:
 *     samconfConfigStatusE_t:
 *         SAMCONF_CONFIG_OK – on success.
 *         SAMCONF_CONFIG_ERROR – on failure.
 */
samconfConfigStatusE_t samconfEnvBackendClose(samconfConfigBackend_t *backend);

__END_DECLS
