// SPDX-License-Identifier: MIT
#include "samconfPathGetPath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfPathGetPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfPathGetPathEnvSuccess),
        TEST_CASE(samconfTestSamconfPathGetPathJsonSuccess),
        TEST_CASE(samconfTestSamconfPathGetPathErrParamConfig),
        TEST_CASE(samconfTestSamconfPathGetPathErrParamPath),
    };

    return RUN_TEST_SUITE(tests, samconfPathGetPathUtest);
}

static int samconfPathGetPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfPathGetPathUtestTeardown(UNUSED void **state) {
    return 0;
}
