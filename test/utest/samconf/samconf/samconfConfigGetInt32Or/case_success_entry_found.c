// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

typedef struct {
    const char *path;
    int32_t value;
} _pathVal_t;

void samconfTestSamconfConfigGetInt32OrSuccessEntryFound(void **state) {
    samconfConfig_t *testConfig = *state;
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};

    _pathVal_t tests[] = {{.path = "root/elos/ClientInputs/Plugins/LocalTcpClient/Config/Port", .value = 54321},
                          {.path = "root/elos/ClientInputs/Plugins/PublicTcpClient/Config/Port", .value = 5422}};

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the correct value from the config");

    for (size_t t = 0; t < ARRAY_SIZE(tests); t++) {
        for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
            result = samconfConfigGetInt32Or(testConfig, tests[t].path, defaultValues[i]);
            assert_int_equal(result, tests[t].value);
        }
    }
}
