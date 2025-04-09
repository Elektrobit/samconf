// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateIntAt_utest.h"

int samconfTestSamconfCreateIntAtErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateIntAtErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateIntAtErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";
    int64_t testValue = 42;

    TEST("samconfCreateIntAt");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfCreateIntAt(NULL, testPath, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
