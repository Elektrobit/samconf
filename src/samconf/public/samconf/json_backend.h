// SPDX-License-Identifier: MIT
#ifndef SAMCONF_JSON_BACKEND_H
#define SAMCONF_JSON_BACKEND_H

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

#define DECIMAL_DIGITS_BOUND(t) (241 * sizeof(t) / 100 + 2)

extern const samconfConfigBackendOps_t samconfJsonOps;

__BEGIN_DECLS

samconfConfigStatusE_t samconfJsonBackendOpen(const char *location, samconfConfigBackend_t *backend);

samconfConfigStatusE_t samconfJsonBackendLoad(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);

samconfConfigStatusE_t samconfJsonBackendSupports(const char *location, bool *isSupported);

samconfConfigStatusE_t samconfJsonBackendClose(samconfConfigBackend_t *backend);

/*******************************************************************
 * generate a json_object out of a samconfConfig_t
 *
 * Parameters:
 *     config : the pointer in which to strore the resulting config
 *
 * Returns:
 *         the config as a json_object struct
 ******************************************************************/
struct json_object *samconfDumpJsonConfig(const samconfConfig_t *const config);

__END_DECLS

#endif /* SAMCONF_JSON_BACKEND_H */
