// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrErrorRootNull(UNUSED void **state) {
    bool result;
    bool defaultBool;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the provided default value since no config was provided");

    defaultBool = true;
    result = samconfConfigGetBoolOr(root, testPath, defaultBool);
    assert_int_equal(result, defaultBool);
    assert_true(result);

    defaultBool = false;
    result = samconfConfigGetBoolOr(root, testPath, defaultBool);
    assert_int_equal(result, defaultBool);
    assert_false(result);
}
