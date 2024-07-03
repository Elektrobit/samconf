// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrSuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrSuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrSuccessEntryNotFound(void **state) {
    samconfConfig_t *testConfig = *state;
    bool result;
    bool defaultValue;
    const char *testPaths[] = {"",
                               "elos/Scanner/SysLongLong",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/"};

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the providet default value since target entry specified in path does not exist");

    defaultValue = false;
    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        result = samconfConfigGetBoolOr(testConfig, testPaths[h], defaultValue);
        assert_int_equal(defaultValue, result);
        assert_false(result);
    }
    defaultValue = true;
    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        result = samconfConfigGetBoolOr(testConfig, testPaths[h], defaultValue);
        assert_int_equal(defaultValue, result);
        assert_true(result);
    }
}
