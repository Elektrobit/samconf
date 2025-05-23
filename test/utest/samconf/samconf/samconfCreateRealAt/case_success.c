// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateRealAt_utest.h"

int samconfTestSamconfCreateRealAtSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateRealAtSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateRealAtSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;
    double testValue = 1.9865;

    TEST("samconfCreateRealAt");
    SHOULD("%s", "create node with real value and insert into existing config");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateRealAt(&testConfig, "UTEST/VARIABLE/REAL", testValue);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigGet(testConfig, "UTEST/VARIABLE/REAL", &testnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testnode->key, "REAL");
    assert_double_equal(testnode->value.real, 1.9865, 0.0000);
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
