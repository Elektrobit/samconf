// SPDX-License-Identifier: MIT

#include "samconfPathGetPathUntil_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfPathGetPathUntilUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfPathGetPathUntilSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfPathGetPathUntilUtest);
}

static int samconfPathGetPathUntilUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfPathGetPathUntilUtestTeardown(UNUSED void **state) {
    return 0;
}
