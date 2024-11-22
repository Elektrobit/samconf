// SPDX-License-Identifier: MIT
#include "samconfCreateFromStringAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCreateFromStringAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCreateFromStringAtSuccess),
        TEST_CASE(samconfTestSamconfCreateFromStringAtErrorNullPath),
        TEST_CASE(samconfTestSamconfCreateFromStringAtErrorNullRoot),
        TEST_CASE(samconfTestSamconfCreateFromStringAtErrorNullValue),
        TEST_CASE(samconfTestSamconfCreateFromStringAtExtErrorConfigNew),
    };

    return RUN_TEST_SUITE(tests, samconfCreateFromStringAtUtest);
}

static int samconfCreateFromStringAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCreateFromStringAtUtestTeardown(UNUSED void **state) {
    return 0;
}
