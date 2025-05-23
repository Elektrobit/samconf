// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/defines.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <stdio.h>
#include <stdlib.h>

#include "dltConfig.h"
#include "jsonConfig.h"
#include "safu/common.h"
#include "samconfConfigMergeConfigs_utest.h"
#include "sqlConfig.h"

int samconfTestSamconfConfigMergeConfigsSignageSuccessSetup(UNUSED void **state) {
    samconfConfigStatusE_t result;

    samconfTestConfigs_t *testConfigs = safuAllocMem(NULL, sizeof(*testConfigs));
    assert_non_null(testConfigs);

    samconfConfig_t *defConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_DEF, true, defConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    samconfConfig_t *fetchapiConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_FETCHAPI, false, fetchapiConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    samconfConfig_t *jsonConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_JSON, true, jsonConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    samconfConfig_t *sqlConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_SQL, false, sqlConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    samconfConfig_t *dltConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG_DLT, true, dltConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    testConfigs->defConfig = defConfig;
    testConfigs->fetchapiConfig = fetchapiConfig;
    testConfigs->jsonConfig = jsonConfig;
    testConfigs->sqlConfig = sqlConfig;
    testConfigs->dltConfig = dltConfig;

    *state = testConfigs;

    return 0;
}

int samconfTestSamconfConfigMergeConfigsSignageSuccessTeardown(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfTestConfigs_t *testConfigs = *state;

    result = samconfConfigDelete(testConfigs->defConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigDelete(testConfigs->fetchapiConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigDelete(testConfigs->jsonConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigDelete(testConfigs->sqlConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigDelete(testConfigs->dltConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    free(testConfigs);
    return 0;
}

void samconfTestSamconfConfigMergeConfigsSignageSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    const samconfConfig_t *testResult = NULL;
    samconfTestConfigs_t *testConfigs = *state;
    samconfConfig_t *testConfigsToMerge[] = {testConfigs->fetchapiConfig, testConfigs->jsonConfig,
                                             testConfigs->sqlConfig, testConfigs->dltConfig};
    size_t testConfigCount = ARRAY_SIZE(testConfigsToMerge);

    TEST("samconfConfigMergeConfigs");
    SHOULD("%s", "merge given configs to root config according to signage");

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/fetchapi", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/JsonBackend", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/SQLBackend", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/DLT", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigMergeConfigs(&testConfigs->defConfig, testConfigsToMerge, testConfigCount);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/fetchapi", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/JsonBackend", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/SQLBackend", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);

    result = samconfConfigGet(testConfigs->defConfig, "root/elos/EventLogging/Plugins/DLT", &testResult);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
