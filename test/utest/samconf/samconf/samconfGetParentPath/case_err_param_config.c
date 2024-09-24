// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfGetParentPath_utest.h"

int samconfTestSamconfGetParentPathErrParamConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfGetParentPathErrParamConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfGetParentPathErrParamConfig(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testpath = NULL;

    TEST("samconfGetParentPath");
    SHOULD("%s", "not return expected path to given null config");

    result = samconfGetParentPath(NULL, &testpath);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
