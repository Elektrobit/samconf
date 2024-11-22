// SPDX-License-Identifier: MIT
#include "samconfCreateBoolAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCreateBoolAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCreateBoolAtSuccess),
        TEST_CASE(samconfTestSamconfCreateBoolAtErrorNullPath),
        TEST_CASE(samconfTestSamconfCreateBoolAtErrorNullRoot),
        TEST_CASE(samconfTestSamconfCreateBoolAtExtErrorConfigNew),
    };

    return RUN_TEST_SUITE(tests, samconfCreateBoolAtUtest);
}

static int samconfCreateBoolAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCreateBoolAtUtestTeardown(UNUSED void **state) {
    return 0;
}
