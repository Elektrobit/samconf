// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfEnvBackendLoad_utest.h"

int samconfTestSamconfEnvBackendLoadExtErrorCreateConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendLoadExtErrorCreateConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendLoadExtErrorCreateConfig(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfigBackend_t *testBackend = NULL;
    samconfConfig_t *testroot = NULL;

    TEST("samconfEnvBackendLoad");
    SHOULD("%s", "fail on new config creation");

    MOCK_FUNC_AFTER_CALL(samconfEnvBackendCreateConfig, 0);
    expect_any(samconfEnvBackendCreateConfig, root);
    expect_any(samconfEnvBackendCreateConfig, path);
    expect_any(samconfEnvBackendCreateConfig, value);
    will_return(samconfEnvBackendCreateConfig, testroot);
    will_return(samconfEnvBackendCreateConfig, SAMCONF_CONFIG_ERROR);

    result = samconfEnvBackendLoad(testBackend, true, &testroot);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
