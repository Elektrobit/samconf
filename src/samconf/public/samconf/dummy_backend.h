// SPDX-License-Identifier: MIT
#pragma once

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

extern const samconfConfigBackendOps_t samconfDummyOps;

__BEGIN_DECLS

samconfConfigStatusE_t samconfSupportsDummy(UNUSED const char *location, UNUSED bool *isSupported);
samconfConfigStatusE_t samconfOpenDummy(UNUSED const char *location, UNUSED samconfConfigBackend_t *backend);
samconfConfigStatusE_t samconfLoadDummy(UNUSED samconfConfigBackend_t *backend, UNUSED bool isSigned,
                                               UNUSED samconfConfig_t **config);
samconfConfigStatusE_t samconfCloseDummy(UNUSED samconfConfigBackend_t *backend);

__END_DECLS
