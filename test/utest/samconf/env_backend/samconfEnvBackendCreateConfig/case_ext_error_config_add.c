// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfEnvBackendCreateConfig_utest.h"

int samconfTestSamconfEnvBackendCreateConfigExtErrorConfigAddSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCreateConfigExtErrorConfigAddTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCreateConfigExtErrorConfigAdd(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";

    TEST("samconfEnvBackendCreateConfig");
    SHOULD("%s", "node not added to root config as node addition fails");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigAdd, 0);
    expect_any(samconfConfigAdd, parent);
    expect_any(samconfConfigAdd, child);
    will_return(samconfConfigAdd, SAMCONF_CONFIG_ERROR);

    result = samconfEnvBackendCreateConfig(&testConfig, testPath, "42");
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
