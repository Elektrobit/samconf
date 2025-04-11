// SPDX-License-Identifier: MIT
#include <samconf/samconf.h>
#include <stdlib.h>

#include "samconfEnvBackendLoad_utest.h"

int samconfTestSamconfEnvBackendLoadSuccessSetup(void **state) {
    setenv("UTEST_VARIABLE_STRING", "utest string", 1);
    setenv("UTEST_VARIABLE_INT", "42", 1);
    setenv("UTEST_VARIABLE_REAL", "1.9865", 1);
    setenv("UTEST_VARIABLE_BOOL", "true", 1);
    setenv("UTEST_UTEST_VARIABLE_INT", "42", 1);

    samconfConfigStatusE_t result;
    samconfUri_t *uri;
    result = samconfUriNew(&uri, "env://?envPrefix=UTEST_VARIABLE&testVar=Check");
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    samconfConfigBackend_t *backend = malloc(sizeof(samconfConfigBackend_t));
    if (backend == NULL) {
        return 1;
    }

    samconfEnvBackendLoadTestData_t *data = malloc(sizeof(samconfEnvBackendLoadTestData_t));
    if (data == NULL) {
        free(backend);
        return 1;
    }

    data->backend = backend;
    backend->backendHandle = uri;
    *state = data;
    return 0;
}

int samconfTestSamconfEnvBackendLoadSuccessTeardown(void **state) {
    unsetenv("UTEST_VARIABLE_STRING");
    unsetenv("UTEST_VARIABLE_INT");
    unsetenv("UTEST_VARIABLE_REAL");
    unsetenv("UTEST_VARIABLE_BOOL");
    unsetenv("UTEST_UTEST_VARIABLE_INT");

    samconfConfigStatusE_t result;
    samconfEnvBackendLoadTestData_t *data = (samconfEnvBackendLoadTestData_t *)*state;

    result = samconfUriDelete(data->backend->backendHandle);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    free(data->backend);

    result = samconfConfigDelete(data->config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    free(data);

    return 0;
}

void samconfTestSamconfEnvBackendLoadSuccess(void **state) {
    samconfEnvBackendLoadTestData_t *data = (samconfEnvBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testnode = NULL;

    const char *testPaths[] = {"UTEST/VARIABLE/STRING", "UTEST/VARIABLE/INT", "UTEST/VARIABLE/REAL",
                               "UTEST/VARIABLE/BOOL", "UTEST/UTEST/VARIABLE/INT"};

    const char *resultKey[] = {"STRING", "INT", "REAL", "BOOL", "INT"};
    samconfEnvBackendTestValues_t testValues = {.string = "utest string", .integer = 42, .real = 1.9865};

    TEST("samconfEnvBackendLoad");
    SHOULD("%s", "return a config_t object containing env variable values");

    result = samconfEnvBackendLoad(data->backend, true, &data->config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        PARAM("%s", testPaths[i]);
        result = samconfConfigGet(data->config, testPaths[i], &testnode);
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
}
