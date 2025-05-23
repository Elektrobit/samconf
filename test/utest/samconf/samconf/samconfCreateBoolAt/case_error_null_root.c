// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateBoolAt_utest.h"

int samconfTestSamconfCreateBoolAtErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateBoolAtErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateBoolAtErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";
    double testValue = 1.9865;

    TEST("samconfCreateBoolAt");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfCreateBoolAt(NULL, testPath, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
