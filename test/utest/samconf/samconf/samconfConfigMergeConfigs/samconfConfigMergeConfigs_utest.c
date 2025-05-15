// SPDX-License-Identifier: MIT
#include "samconfConfigMergeConfigs_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigMergeConfigsUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigMergeConfigsSuccess),
                                       TEST_CASE(samconfTestSamconfConfigMergeConfigsSignageSuccess)};

    return RUN_TEST_SUITE(tests, samconfConfigMergeConfigsUtest);
}

static int samconfConfigMergeConfigsUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigMergeConfigsUtestTeardown(UNUSED void **state) {
    return 0;
}
