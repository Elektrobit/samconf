// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfCreateStringAt_utest.h"

int samconfTestSamconfCreateStringAtErrorNullRootSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfCreateStringAtErrorNullRootTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfCreateStringAtErrorNullRoot(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testPath = "UTEST/TEST/STRING";
    const char *testValue = "utest string";

    TEST("samconfCreateStringAt");
    SHOULD("%s", "node not added to config as root config is null");

    result = samconfCreateStringAt(NULL, testPath, testValue);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
