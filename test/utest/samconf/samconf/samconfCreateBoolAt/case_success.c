// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateBoolAt_utest.h"

int samconfTestSamconfCreateBoolAtSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateBoolAtSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateBoolAtSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;
    bool testValue = true;

    TEST("samconfCreateBoolAt");
    SHOULD("%s", "create node with boolean value and insert into existing config");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateBoolAt(&testConfig, "UTEST/VARIABLE/BOOL", testValue);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigGet(testConfig, "UTEST/VARIABLE/BOOL", &testnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testnode->key, "BOOL");
    assert_true(testnode->value.boolean);
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
