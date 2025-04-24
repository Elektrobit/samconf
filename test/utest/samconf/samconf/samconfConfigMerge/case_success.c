// SPDX-License-Identifier: MIT
#include <samconf/samconf_types.h>
#include "samconfConfigMerge_utest.h"

int samconfTestSamconfConfigMergeSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigMergeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigMergeSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testMergeConfig = NULL;
    samconfConfig_t *testConfigToMerge = NULL;

    TEST("samconfConfigMerge");
    SHOULD("%s", "merge given config to root config");

    result = samconfMergeConfig(testMergeConfig, testConfigToMerge);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
