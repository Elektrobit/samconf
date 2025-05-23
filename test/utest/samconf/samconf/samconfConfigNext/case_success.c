// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/samconf_types.h>

#include "samconf/test_utils.h"
#include "samconfConfigNext_utest.h"

int samconfTestSamconfConfigNextSuccessSetup(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testConfig = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG, false, testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    *state = testConfig;

    return 0;
}

int samconfTestSamconfConfigNextSuccessTeardown(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testConfig = *(samconfConfig_t **)state;
    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    return 0;
}

void samconfTestSamconfConfigNextSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const samconfConfig_t *testConfig = *state;
    const samconfConfig_t *testConfigToFind = NULL;
    const samconfConfig_t *testNextConfig = NULL;

    TEST("samconfConfigNext");
    SHOULD("%s", "give next config");

    PARAM("Next Node Of Root Node");
    testConfigToFind = testConfig;
    result = samconfConfigNext(testConfig, testConfigToFind, &testNextConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testNextConfig->key, "root");

    PARAM("Next Node Of Last Node");
    testConfigToFind = NULL;
    testNextConfig = NULL;
    result = samconfConfigGet(testConfig, "root/elos/EventLogging/Plugins/DLT/Config/AppId", &testConfigToFind);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigNext(testConfig, testConfigToFind, &testNextConfig);
    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);
    assert_null(testNextConfig);

    PARAM("Next Node of Nth Node");
    testConfigToFind = NULL;
    testNextConfig = NULL;
    result = samconfConfigGet(testConfig, "root/elos/EventLogging/Plugins", &testConfigToFind);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    result = samconfConfigNext(testConfig, testConfigToFind, &testNextConfig);
    assert_string_equal(testNextConfig->key, "fetchapi");
}
