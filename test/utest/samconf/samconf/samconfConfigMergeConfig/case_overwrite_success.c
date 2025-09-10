// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>

#include "fetchapiConfig.h"
#include "samconfConfigMergeConfig_utest.h"

int samconfTestSamconfConfigMergeConfigOverwriteSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigMergeConfigOverwriteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigMergeConfigOverwriteSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testDefConfig = NULL;
    samconfConfig_t *testConfigToMerge = NULL;
    const samconfConfig_t *testResult = NULL;

    TEST("samconfConfigMergeConfig");
    SHOULD("%s", "overwrite value of one config to the other");

    testConfigToMerge = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_FETCHAPI_B, false, testConfigToMerge);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    testDefConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_FETCHAPI, false, testDefConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result =
        samconfConfigGet(testConfigToMerge, "root/elos/EventLogging/Plugins/fetchapi/Config/BufferSize", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigSetInt((samconfConfig_t *)testResult, 200);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigMergeConfig(&testDefConfig, testConfigToMerge);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testDefConfig, "root/elos/EventLogging/Plugins/fetchapi/Config/BufferSize", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    assert_int_equal(testResult->value.integer, 200);

    result = samconfConfigDelete(testDefConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigDelete(testConfigToMerge);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
