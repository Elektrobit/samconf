// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrExtErrConfigGetSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrExtErrConfigGetTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32OrExtErrConfigGet(void **state) {
    samconfConfig_t *testConfig = *state;
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};

    const char *testPath = "Scanner/Syslog/SyslogPath";

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the providet default value after samconfConfigGet internaly failed");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
        expect_value(samconfConfigGet, root, testConfig);
        expect_value(samconfConfigGet, path, testPath);
        expect_any(samconfConfigGet, result);

        will_return(samconfConfigGet, NULL);
        will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

        result = samconfConfigGetInt32Or(testConfig, testPath, defaultValues[i]);
        assert_int_equal(result, defaultValues[i]);
    }
}
