// SPDX-License-Identifier: MIT
#include "samconf/dummy_backend.h"

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

samconfConfigStatusE_t samconfSupportsDummy(UNUSED const char *location, UNUSED bool *isSupported) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

samconfConfigStatusE_t samconfOpenDummy(UNUSED const char *location, UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

samconfConfigStatusE_t samconfLoadDummy(UNUSED samconfConfigBackend_t *backend, UNUSED bool isSigned,
                                        UNUSED samconfConfig_t **config) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

samconfConfigStatusE_t samconfCloseDummy(UNUSED samconfConfigBackend_t *backend) {
    return SAMCONF_CONFIG_NOT_IMPLEMENTED;
}

const samconfConfigBackendOps_t samconfDummyOps = {
    .supports = samconfSupportsDummy,
    .open = samconfOpenDummy,
    .load = samconfLoadDummy,
    .close = samconfCloseDummy,
};
