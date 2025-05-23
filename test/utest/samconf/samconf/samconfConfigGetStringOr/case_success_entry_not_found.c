// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrSuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrSuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringOrSuccessEntryNotFound(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *defaultValue = "foo";
    const char *testPaths[] = {"elos/Scanner/SysLongLong", "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting", "NotExisting", "NotExisting/"};

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the providet default value since target entry specified in path does not exist");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        result = samconfConfigGetStringOr(testConfig, testPaths[h], defaultValue);
        assert_string_equal(result, defaultValue);
    }
}
