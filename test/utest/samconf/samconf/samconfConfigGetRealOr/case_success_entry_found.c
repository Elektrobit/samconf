// SPDX-License-Identifier: MIT
#include <math.h>
#include <stdint.h>

#include "samconfConfigGetRealOr_utest.h"

int samconfTestSamconfConfigGetRealOrSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealOrSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

typedef struct {
    const char *path;
    double value;
} _pathVal_t;

void samconfTestSamconfConfigGetRealOrSuccessEntryFound(void **state) {
    samconfConfig_t *testConfig = *state;
    double result = NAN;
    double defaultValues[] = {0.0, -12.3, DBL_MAX, DBL_MIN, 42.7, DBL_EPSILON, DBL_MAX_EXP, DBL_MIN_EXP};

    _pathVal_t tests[] = {
        {.path = "root/elos/ClientInputs/Plugins/LocalTcpClient/Config/ADoubleValue", .value = 4.7},
        {.path = "root/elos/ClientInputs/Plugins/PublicTcpClient/Config/ADoubleValue", .value = 33.009}};

    TEST("samconfConfigGetRealOr");
    SHOULD("%s", "return the correct value from the config");

    for (size_t t = 0; t < ARRAY_SIZE(tests); t++) {
        for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
            result = samconfConfigGetRealOr(testConfig, tests[t].path, defaultValues[i]);
            assert_double_equal(result, tests[t].value, DBL_EPSILON);
        }
    }
}
