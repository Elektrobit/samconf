// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/samconf_types.h>

#include "samconf/test_utils.h"
#include "samconfConfigNext_utest.h"

int samconfTestSamconfConfigNextErrorSetup(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG, false, testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    *state = testConfig;

    return 0;
}

int samconfTestSamconfConfigNextErrorTeardown(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testConfig = *(samconfConfig_t **)state;
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    return 0;
}

void samconfTestSamconfConfigNextError(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const samconfConfig_t *testConfig = *state;
    const samconfConfig_t *testConfigToFind = NULL;
    const samconfConfig_t *testNextConfig = NULL;

    TEST("samconfConfigNext");
    SHOULD("%s", "return NULL since root is NULL");

    PARAM("Root is null");
    result = samconfConfigGet(testConfig, "root/elos/EventLogging/Plugins/DLT/Config/AppId", &testConfigToFind);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigNext(NULL, testConfigToFind, &testNextConfig);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_null(testNextConfig);

    PARAM("Node to find is null");
    result = samconfConfigNext(testConfig, NULL, &testNextConfig);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_null(testNextConfig);

    PARAM("Next Config is known");
    result = samconfConfigGet(testConfig, "root/elos/EventLogging", &testConfigToFind);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    testNextConfig = testConfigToFind;
    result = samconfConfigNext(testConfig, testConfigToFind, &testNextConfig);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
