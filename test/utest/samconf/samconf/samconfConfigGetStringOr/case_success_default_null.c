// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrSuccessDefaultNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrSuccessDefaultNullTeardown(UNUSED void **state) {
    return 0;
}

typedef struct {
    const char *path;
    const char *value;
} _pathVal_t;

void samconfTestSamconfConfigGetStringOrSuccessDefaultNull(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *testPaths[] = {"",
                               "elos/Scanner/SysLongLong",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/",
                               "root/elos/ClientInputs/Plugins/LocalTcpClient/Config/Port"};

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return NULL if no string can be found under that path and the string otherwise");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        result = samconfConfigGetStringOr(testConfig, testPaths[h], NULL);
        assert_null(result);
    }

    _pathVal_t tests[] = {{.path = "root/elos/ClientInputs/Plugins/LocalTcpClient/File", .value = "client_dummy.so"},
                          {.path = "root/elos/ClientInputs/Plugins/PublicTcpClient/Run", .value = "always"}};
    for (size_t t = 0; t < ARRAY_SIZE(tests); t++) {
        result = samconfConfigGetStringOr(testConfig, tests[t].path, NULL);
        assert_string_equal(result, tests[t].value);
    }
}
