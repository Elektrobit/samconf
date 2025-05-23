// SPDX-License-Identifier: MIT
#include "samconfConfigNext_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigNextUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigNextSuccess),
                                       TEST_CASE(samconfTestSamconfConfigNextError)};

    return RUN_TEST_SUITE(tests, samconfConfigNextUtest);
}

static int samconfConfigNextUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigNextUtestTeardown(UNUSED void **state) {
    return 0;
}
