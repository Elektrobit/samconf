// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateRealAt_utest.h"

int samconfTestSamconfCreateRealAtErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateRealAtErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateRealAtErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    double testValue = 1.9865;

    TEST("samconfCreateRealAt");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateRealAt(&testConfig, NULL, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
