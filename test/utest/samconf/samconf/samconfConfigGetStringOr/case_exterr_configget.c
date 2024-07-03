// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrExtErrConfigGetSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrExtErrConfigGetTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringOrExtErrConfigGet(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *defaultValues[] = {
        "",
        "foo",
    };

    const char *testPath = "Scanner/Syslog/SyslogPath";

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the providet default value after samconfConfigGet internaly failed");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
        expect_value(samconfConfigGet, root, testConfig);
        expect_value(samconfConfigGet, path, testPath);
        expect_any(samconfConfigGet, result);

        will_return(samconfConfigGet, NULL);
        will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

        result = samconfConfigGetStringOr(testConfig, testPath, defaultValues[i]);
        assert_string_equal(result, defaultValues[i]);
    }
}
