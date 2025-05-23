// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateBoolAt_utest.h"

int samconfTestSamconfCreateBoolAtErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateBoolAtErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateBoolAtErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    double testValue = 1.9865;

    TEST("samconfCreateBoolAt");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateBoolAt(&testConfig, NULL, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
