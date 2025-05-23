// SPDX-License-Identifier: MIT
#include "samconfCreateIntAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCreateIntAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCreateIntAtSuccess),
        TEST_CASE(samconfTestSamconfCreateIntAtErrorNullPath),
        TEST_CASE(samconfTestSamconfCreateIntAtErrorNullRoot),
        TEST_CASE(samconfTestSamconfCreateIntAtExtErrorConfigNew),
    };

    return RUN_TEST_SUITE(tests, samconfCreateIntAtUtest);
}

static int samconfCreateIntAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCreateIntAtUtestTeardown(UNUSED void **state) {
    return 0;
}
