// SPDX-License-Identifier: MIT
#include "samconfCreateRealAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCreateRealAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCreateRealAtSuccess),
        TEST_CASE(samconfTestSamconfCreateRealAtErrorNullPath),
        TEST_CASE(samconfTestSamconfCreateRealAtErrorNullRoot),
        TEST_CASE(samconfTestSamconfCreateRealAtExtErrorConfigNew),
    };

    return RUN_TEST_SUITE(tests, samconfCreateRealAtUtest);
}

static int samconfCreateRealAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCreateRealAtUtestTeardown(UNUSED void **state) {
    return 0;
}
