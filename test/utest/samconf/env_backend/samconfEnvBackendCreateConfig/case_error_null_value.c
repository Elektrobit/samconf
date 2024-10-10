// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendCreateConfig_utest.h"

int samconfTestSamconfEnvBackendCreateConfigErrorNullValueSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCreateConfigErrorNullValueTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCreateConfigErrorNullValue(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";
    const char *testKey = "INTEGER";
    const samconfConfig_t *testnode = NULL;

    TEST("samconfEnvBackendCreateConfig");
    SHOULD("%s", "node type set to default as value is null");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfEnvBackendCreateConfig(&testConfig, testPath, NULL);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testConfig, testPath, &testnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    assert_string_equal(testnode->key, testKey);
    assert_int_equal(testnode->type, SAMCONF_CONFIG_VALUE_OBJECT);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
