// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendCreateConfig_utest.h"

int samconfTestSamconfEnvBackendCreateConfigSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCreateConfigSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCreateConfigSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;

    const char *testPaths[] = {"UTEST/VARIABLE/STRING", "UTEST/VARIABLE/INT",       "UTEST/VARIABLE/REAL",
                               "UTEST/VARIABLE/BOOL",   "UTEST/UTEST/VARIABLE/INT", "UTEST/VARIABLE/INT"};

    const char *testValue[] = {"utest string", "42", "1.9865", "true", "42", "42"};

    const char *testKey[] = {"STRING", "INT", "REAL", "BOOL", "INT", "INT"};

    TEST("samconfEnvBackendCreateConfig");
    SHOULD("%s", "add node to existing config and return it");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        PARAM("%s", testPaths[i]);
        result = samconfEnvBackendCreateConfig(&testConfig, testPaths[i], testValue[i]);
        assert_int_equal(result, SAMCONF_CONFIG_OK);
        result = samconfConfigGet(testConfig, testPaths[i], &testnode);
        assert_int_equal(result, SAMCONF_CONFIG_OK);
        assert_string_equal(testnode->key, testKey[i]);
        if (testnode->type == SAMCONF_CONFIG_VALUE_INT) {
            assert_int_equal(testnode->value.integer, 42);
        } else if (testnode->type == SAMCONF_CONFIG_VALUE_BOOLEAN) {
            assert_int_equal(testnode->value.boolean, true);
        } else if (testnode->type == SAMCONF_CONFIG_VALUE_REAL) {
            assert_int_equal(testnode->value.real, 1.9865);
        } else {
            assert_string_equal(testnode->value.string, "utest string");
        }
    }
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
