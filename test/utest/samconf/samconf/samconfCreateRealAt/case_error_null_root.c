// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateRealAt_utest.h"

int samconfTestSamconfCreateRealAtErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateRealAtErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateRealAtErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/REAL";
    double testValue = 1.9865;

    TEST("samconfCreateRealAt");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfCreateRealAt(NULL, testPath, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
