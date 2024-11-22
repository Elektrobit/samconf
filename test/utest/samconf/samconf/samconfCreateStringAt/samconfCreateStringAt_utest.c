// SPDX-License-Identifier: MIT
#include "samconfCreateStringAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCreateStringAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCreateStringAtSuccess),
        TEST_CASE(samconfTestSamconfCreateStringAtErrorNullPath),
        TEST_CASE(samconfTestSamconfCreateStringAtErrorNullRoot),
        TEST_CASE(samconfTestSamconfCreateStringAtExtErrorConfigNew),
    };

    return RUN_TEST_SUITE(tests, samconfCreateStringAtUtest);
}

static int samconfCreateStringAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCreateStringAtUtestTeardown(UNUSED void **state) {
    return 0;
}
