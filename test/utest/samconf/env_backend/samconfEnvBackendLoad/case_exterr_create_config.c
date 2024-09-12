// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfEnvBackendLoad_utest.h"

int samconfTestSamconfEnvBackendLoadExtErrorCreateConfigSetup(void **state) {
    setenv("UTEST_VARIABLE_STRING", "utest string", 1);

    samconfConfigStatusE_t result;
    samconfUri_t *uri;
    result = samconfUriNew(&uri, "env://UTEST_VARIABLE");
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

int samconfTestSamconfEnvBackendLoadExtErrorCreateConfigTeardown(void **state) {
    unsetenv("UTEST_VARIABLE_STRING");

    samconfConfigStatusE_t result;
    samconfEnvBackendLoadTestData_t *data = (samconfEnvBackendLoadTestData_t *)*state;

    result = samconfUriDelete(data->backend->backendHandle);
    free(data->backend);

    result = samconfConfigDelete(data->config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    free(data);
    return 0;
}

void samconfTestSamconfEnvBackendLoadExtErrorCreateConfig(void **state) {
    samconfEnvBackendLoadTestData_t *data = (samconfEnvBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;

    TEST("samconfEnvBackendLoad");
    SHOULD("%s", "fail on new config creation");

    MOCK_FUNC_AFTER_CALL(samconfEnvBackendCreateConfig, 0);
    expect_any(samconfEnvBackendCreateConfig, root);
    expect_any(samconfEnvBackendCreateConfig, path);
    expect_any(samconfEnvBackendCreateConfig, value);
    will_return(samconfEnvBackendCreateConfig, data->config);
    will_return(samconfEnvBackendCreateConfig, SAMCONF_CONFIG_ERROR);

    result = samconfEnvBackendLoad(data->backend, true, &data->config);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
