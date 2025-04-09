// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateFromStringAt_utest.h"

int samconfTestSamconfCreateFromStringAtErrorNullValueSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateFromStringAtErrorNullValueTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateFromStringAtErrorNullValue(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";

    TEST("samconfCreateFromStringAt");
    SHOULD("%s", "node type set to default as value is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateFromStringAt(&testConfig, testPath, NULL);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
