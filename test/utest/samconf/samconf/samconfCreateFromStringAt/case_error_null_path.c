// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateFromStringAt_utest.h"

int samconfTestSamconfCreateFromStringAtErrorNullPathSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateFromStringAtErrorNullPathTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateFromStringAtErrorNullPath(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;

    TEST("samconfCreateFromStringAt");
    SHOULD("%s", "node not added to config as path is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateFromStringAt(&testConfig, NULL, "42");
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
