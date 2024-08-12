// SPDX-License-Identifier: MIT
#pragma once

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

#define DECIMAL_DIGITS_BOUND(t) (241 * sizeof(t) / 100 + 2)

extern const samconfConfigBackendOps_t samconfEnvOps;

__BEGIN_DECLS

samconfConfigStatusE_t samconfEnvBackendOpen(const char *location, samconfConfigBackend_t *backend);

samconfConfigStatusE_t samconfEnvBackendCreateConfig(samconfConfig_t **root, const char *path, const char *value);

samconfConfigStatusE_t samconfEnvBackendLoad(samconfConfigBackend_t *backend, bool isSigned, samconfConfig_t **config);

samconfConfigStatusE_t samconfEnvBackendSupports(const char *location, bool *isSupported);

samconfConfigStatusE_t samconfEnvBackendClose(samconfConfigBackend_t *backend);

__END_DECLS
