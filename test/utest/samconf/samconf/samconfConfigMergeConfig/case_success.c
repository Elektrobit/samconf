// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include "samconfConfigMergeConfig_utest.h"

int samconfTestSamconfConfigMergeConfigSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigMergeConfigSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigMergeConfigSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testDefConfig = NULL;
    samconfConfig_t *testConfigToMerge = NULL;
    const samconfConfig_t *testResult = NULL;
    bool defConfigSignage[2] = {true, false};
    bool mergeConfigSignage[2] = {true, false};

    TEST("samconfConfigMergeConfig");
    SHOULD("%s", "merge given configs to root config based on signage");

    for (size_t i = 0; i < ARRAY_SIZE(mergeConfigSignage); i++) {
        testConfigToMerge = calloc(1, sizeof(samconfConfig_t));
        result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_FETCHAPI, mergeConfigSignage[i], testConfigToMerge);
        assert_int_equal(result, SAMCONF_CONFIG_OK);

        for (size_t j = 0; j < ARRAY_SIZE(defConfigSignage); j++) {
            PARAM("Default Config %s signed and Merge Config %s signed", (defConfigSignage[j] ? "is" : "is not"),
                  (mergeConfigSignage[i] ? "is" : "is not"));

            testDefConfig = calloc(1, sizeof(samconfConfig_t));
            result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_DEF, defConfigSignage[j], testDefConfig);
            assert_int_equal(result, SAMCONF_CONFIG_OK);

            result = samconfConfigMergeConfig(&testDefConfig, testConfigToMerge);
            if (defConfigSignage[j] && !mergeConfigSignage[i]) {
                assert_int_equal(result, SAMCONF_CONFIG_ERROR);
            } else {
                assert_int_equal(result, SAMCONF_CONFIG_OK);
            }

            result = samconfConfigGet(testDefConfig, "root/elos/EventLogging/Plugins/fetchapi", &testResult);
            if (defConfigSignage[j] && !mergeConfigSignage[i]) {
                assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);
            } else {
                assert_int_equal(result, SAMCONF_CONFIG_OK);
            }

            result = samconfConfigDelete(testDefConfig);
            assert_int_equal(result, SAMCONF_CONFIG_OK);
        }

        result = samconfConfigDelete(testConfigToMerge);
        assert_int_equal(result, SAMCONF_CONFIG_OK);
    }
}
