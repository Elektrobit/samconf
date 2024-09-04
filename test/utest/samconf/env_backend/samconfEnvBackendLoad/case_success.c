// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendLoad_utest.h"

int samconfTestSamconfEnvBackendLoadSuccessSetup(UNUSED void **state) {
    setenv("UTEST_VARIABLE_STRING", "utest string", 1);
    setenv("UTEST_VARIABLE_INT", "42", 1);
    setenv("UTEST_VARIABLE_REAL", "1.9865", 1);
    setenv("UTEST_VARIABLE_BOOL", "true", 1);
    setenv("UTEST_UTEST_VARIABLE_INT", "42", 1);
    setenv("__UTEST_VARIABLE_INT", "42", 1);
    return 0;
}

int samconfTestSamconfEnvBackendLoadSuccessTeardown(UNUSED void **state) {
    unsetenv("UTEST_VARIABLE_STRING");
    unsetenv("UTEST_VARIABLE_INT");
    unsetenv("UTEST_VARIABLE_REAL");
    unsetenv("UTEST_VARIABLE_BOOL");
    unsetenv("UTEST_UTEST_VARIABLE_INT");
    unsetenv("__UTEST_VARIABLE_INT");
    return 0;
}

void samconfTestSamconfEnvBackendLoadSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfigBackend_t *testBackend = NULL;
    samconfConfig_t *testroot = NULL;
    const samconfConfig_t *testnode = NULL;

    const char *testPaths[] = {"UTEST/VARIABLE/STRING", "UTEST/VARIABLE/INT",       "UTEST/VARIABLE/REAL",
                               "UTEST/VARIABLE/BOOL",   "UTEST/UTEST/VARIABLE/INT", "UTEST/VARIABLE/INT"};

    const char *resultKey[] = {"STRING", "INT", "REAL", "BOOL", "INT", "INT"};
    samconfEnvBackendTestValues_t testValues = {.string = "utest string", .integer = 42, .real = 1.9865};

    TEST("samconfEnvBackendLoad");
    SHOULD("%s", "return a config_t object containing env variable values");

    result = samconfEnvBackendLoad(testBackend, true, &testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        PARAM("%s", testPaths[i]);
        result = samconfConfigGet(testroot, testPaths[i], &testnode);
        assert_int_equal(result, SAMCONF_CONFIG_OK);
        assert_string_equal(testnode->key, resultKey[i]);
        switch (testnode->type) {
            case SAMCONF_CONFIG_VALUE_INT:
                assert_int_equal(testnode->value.integer, testValues.integer);
                break;
            case SAMCONF_CONFIG_VALUE_REAL:
                assert_double_equal(testnode->value.real, testValues.real, 0.0000);
                break;
            case SAMCONF_CONFIG_VALUE_BOOLEAN:
                assert_true(testnode->value.boolean);
                break;
            default:
                assert_string_equal(testnode->value.string, testValues.string);
                break;
        }
    }

    result = samconfConfigDelete(testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
