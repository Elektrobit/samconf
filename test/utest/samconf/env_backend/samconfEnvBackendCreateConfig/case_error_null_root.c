// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendCreateConfig_utest.h"

int samconfTestSamconfEnvBackendCreateConfigErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCreateConfigErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCreateConfigErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";

    TEST("samconfEnvBackendCreateConfig");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfEnvBackendCreateConfig(NULL, testPath, "42");
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
