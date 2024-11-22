// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateFromStringAt_utest.h"

int samconfTestSamconfCreateFromStringAtErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateFromStringAtErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateFromStringAtErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/INTEGER";

    TEST("samconfCreateFromStringAt");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfCreateFromStringAt(NULL, testPath, "42");
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
