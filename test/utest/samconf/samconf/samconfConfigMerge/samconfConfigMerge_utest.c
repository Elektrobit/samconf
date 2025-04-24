// SPDX-License-Identifier: MIT
#include "samconfConfigMerge_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigMergeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigMergeSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfConfigMergeUtest);
}

static int samconfConfigMergeUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigMergeUtestTeardown(UNUSED void **state) {
    return 0;
}
