// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

typedef struct {
    const char *path;
    const char *value;
} _pathVal_t;

void samconfTestSamconfConfigGetStringOrSuccessEntryFound(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *defaultValues[] = {
        "",
        "foo",
    };

    _pathVal_t tests[] = {{.path = "root/elos/ClientInputs/Plugins/LocalTcpClient/File", .value = "client_dummy.so"},
                          {.path = "root/elos/ClientInputs/Plugins/PublicTcpClient/Run", .value = "always"}};

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the correct value from the config");

    for (size_t t = 0; t < ARRAY_SIZE(tests); t++) {
        for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
            result = samconfConfigGetStringOr(testConfig, tests[t].path, defaultValues[i]);
            assert_string_equal(result, tests[t].value);
        }
    }
}
