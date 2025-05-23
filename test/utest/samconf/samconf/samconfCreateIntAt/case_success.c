// SPDX-License-Identifier: MIT
#include <stdlib.h>
#include <string.h>

#include "samconfCreateIntAt_utest.h"

int samconfTestSamconfCreateIntAtSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateIntAtSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateIntAtSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;
    int64_t testValue = 42;

    TEST("samconfCreateIntAt");
    SHOULD("%s", "create node with integer value and insert into existing config");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    testConfig->key = strdup("root");
    testConfig->type = SAMCONF_CONFIG_VALUE_OBJECT;

    result = samconfCreateIntAt(&testConfig, "UTEST/VARIABLE/INT", testValue);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigGet(testConfig, "UTEST/VARIABLE/INT", &testnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testnode->key, "INT");
    assert_int_equal(testnode->value.integer, 42);
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
