// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateIntAt_utest.h"

int samconfTestSamconfCreateIntAtErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateIntAtErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateIntAtErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    int64_t testValue = 42;

    TEST("samconfCreateIntAt");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateIntAt(&testConfig, NULL, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
