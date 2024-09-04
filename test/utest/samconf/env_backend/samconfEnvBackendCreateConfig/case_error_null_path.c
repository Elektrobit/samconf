// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendCreateConfig_utest.h"

int samconfTestSamconfEnvBackendCreateConfigErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCreateConfigErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCreateConfigErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;

    TEST("samconfEnvBackendCreateConfig");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfEnvBackendCreateConfig(&testConfig, NULL, "42");
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
