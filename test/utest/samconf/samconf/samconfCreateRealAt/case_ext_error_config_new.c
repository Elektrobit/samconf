// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfCreateRealAt_utest.h"

int samconfTestSamconfCreateRealAtExtErrorConfigNewSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateRealAtExtErrorConfigNewTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateRealAtExtErrorConfigNew(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    const samconfConfig_t *testNode = NULL;
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/REAL";
    double testValue = 1.9865;

    TEST("samconfCreateRealAt");
    SHOULD("%s", "node not added to root config as node creation fails");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigNew, 0);
    expect_any(samconfConfigNew, config);
    will_return(samconfConfigNew, SAMCONF_CONFIG_ERROR);

    result = samconfCreateRealAt(&testConfig, testPath, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigGet(testConfig, testPath, &testNode);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
