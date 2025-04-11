// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateStringAt_utest.h"

int samconfTestSamconfCreateStringAtSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateStringAtSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateStringAtSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;
    const char *testValue = "utest string";

    TEST("samconfCreateStringAt");
    SHOULD("%s", "create node with real value and insert into existing config");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfCreateStringAt(&testConfig, "UTEST/VARIABLE/STRING", testValue);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigGet(testConfig, "UTEST/VARIABLE/STRING", &testnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testnode->key, "STRING");
    assert_string_equal(testnode->value.string, "utest string");
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
