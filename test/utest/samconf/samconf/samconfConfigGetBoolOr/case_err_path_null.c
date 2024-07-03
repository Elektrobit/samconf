// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrErrorPathNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrErrorPathNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrErrorPathNull(void **state) {
    samconfConfig_t *testConfig = *state;
    bool result;
    bool defaultValue = true;
    const char *testPath = NULL;

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the provided default value since input path is null");

    defaultValue = true;
    result = samconfConfigGetBoolOr(testConfig, testPath, defaultValue);
    assert_int_equal(result, defaultValue);

    defaultValue = false;
    result = samconfConfigGetBoolOr(testConfig, testPath, defaultValue);
    assert_int_equal(result, defaultValue);
}
