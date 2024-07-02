// SPDX-License-Identifier: MIT
#include <math.h>
#include <stdint.h>

#include "samconfConfigGetRealOr_utest.h"

int samconfTestSamconfConfigGetRealOrSuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealOrSuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetRealOrSuccessEntryNotFound(void **state) {
    samconfConfig_t *testConfig = *state;
    double result = NAN;
    double defaultValues[] = {0.0, -12.3, DBL_MAX, DBL_MIN, 42.7, DBL_EPSILON, DBL_MAX_EXP, DBL_MIN_EXP};
    const char *testPaths[] = {"",
                               "elos/Scanner/SysLongLong",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/"};

    TEST("samconfConfigGetRealOr");
    SHOULD("%s",
           "return the providet default value since target entry specified "
           "in path does not exist");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
            result = samconfConfigGetRealOr(testConfig, testPaths[h], defaultValues[i]);
            assert_double_equal(result, defaultValues[i], DBL_EPSILON);
        }
    }
}
