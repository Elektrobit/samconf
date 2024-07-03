// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrSuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrSuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32OrSuccessEntryNotFound(void **state) {
    samconfConfig_t *testConfig = *state;
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};
    const char *testPaths[] = {"",
                               "elos/Scanner/SysLongLong",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/"};

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the providet default value since target entry specified in path does not exist");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
            result = samconfConfigGetInt32Or(testConfig, testPaths[h], defaultValues[i]);
            assert_int_equal(result, defaultValues[i]);
        }
    }
}
