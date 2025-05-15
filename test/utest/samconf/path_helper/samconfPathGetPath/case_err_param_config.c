// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfPathGetPath_utest.h"

int samconfTestSamconfPathGetPathErrParamConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfPathGetPathErrParamConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfPathGetPathErrParamConfig(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testpath = NULL;

    TEST("samconfPathGetPath");
    SHOULD("%s", "not return expected path to given null config");

    result = samconfPathGetPath(NULL, &testpath);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
