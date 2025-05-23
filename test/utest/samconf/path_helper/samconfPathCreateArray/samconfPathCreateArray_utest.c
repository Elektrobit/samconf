// SPDX-License-Identifier: MIT

#include "samconfPathCreateArray_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfPathCreateArrayUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfPathCreateArraySuccess),
                                       TEST_CASE(samconfTestSamconfPathCreateArrayError)};

    return RUN_TEST_SUITE(tests, samconfPathCreateArrayUtest);
}

static int samconfPathCreateArrayUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfPathCreateArrayUtestTeardown(UNUSED void **state) {
    return 0;
}
