// SPDX-License-Identifier: MIT
#include "samconfGetParentPath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfGetParentPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfGetParentPathEnvSuccess),
        TEST_CASE(samconfTestSamconfGetParentPathJsonSuccess),
        TEST_CASE(samconfTestSamconfGetParentPathErrParamConfig),
        TEST_CASE(samconfTestSamconfGetParentPathErrParamPath),
    };

    return RUN_TEST_SUITE(tests, samconfGetParentPathUtest);
}

static int samconfGetParentPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfGetParentPathUtestTeardown(UNUSED void **state) {
    return 0;
}
