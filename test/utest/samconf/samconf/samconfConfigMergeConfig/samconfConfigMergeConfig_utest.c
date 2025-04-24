// SPDX-License-Identifier: MIT
#include "samconfConfigMergeConfig_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigMergeConfigUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigMergeConfigSuccess)};

    return RUN_TEST_SUITE(tests, samconfConfigMergeConfigUtest);
}

static int samconfConfigMergeConfigUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigMergeConfigUtestTeardown(UNUSED void **state) {
    return 0;
}
