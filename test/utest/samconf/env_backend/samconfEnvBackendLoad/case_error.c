// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfEnvBackendLoad_utest.h"

int samconfTestSamconfEnvBackendLoadErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendLoadErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendLoadError(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfigBackend_t *testBackend = NULL;
    samconfConfig_t *testroot = NULL;

    TEST("samconfEnvBackendLoad");
    SHOULD("%s", "fail on new config root creation");

    MOCK_FUNC_AFTER_CALL(samconfConfigNew, 0);
    expect_any(samconfConfigNew, config);
    will_return(samconfConfigNew, SAMCONF_CONFIG_ERROR);

    result = samconfEnvBackendLoad(testBackend, true, &testroot);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
