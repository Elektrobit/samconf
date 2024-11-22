// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateStringAt_utest.h"

int samconfTestSamconfCreateStringAtErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateStringAtErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateStringAtErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const char *testValue = "utest string";

    TEST("samconfCreateStringAt");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateStringAt(&testConfig, NULL, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
