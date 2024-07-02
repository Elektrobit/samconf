// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrExtErrConfigGetSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrExtErrConfigGetTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrExtErrConfigGet(void **state) {
    samconfConfig_t *testConfig = *state;
    bool resultBool;
    bool defaultBool;

    const char *testPath = "Scanner/Syslog/SyslogPath";

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the providet default value after samconfConfigGet internaly failed");

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_value(samconfConfigGet, root, testConfig);
    expect_value(samconfConfigGet, path, testPath);
    expect_any(samconfConfigGet, result);

    will_return(samconfConfigGet, NULL);
    will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

    defaultBool = false;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_int_equal(resultBool, defaultBool);
    assert_false(resultBool);

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_value(samconfConfigGet, root, testConfig);
    expect_value(samconfConfigGet, path, testPath);
    expect_any(samconfConfigGet, result);

    will_return(samconfConfigGet, NULL);
    will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

    defaultBool = true;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_int_equal(resultBool, defaultBool);
    assert_true(resultBool);
}
