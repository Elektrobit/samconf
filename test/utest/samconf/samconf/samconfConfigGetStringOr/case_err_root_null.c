// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>

#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringOrErrorRootNull(UNUSED void **state) {
    const char *result;
    const char *defaultValues[] = {
        "",
        "foo",
    };
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the provided default value since no config was provided");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetStringOr(root, testPath, defaultValues[i]);
        assert_string_equal(result, defaultValues[i]);
    }
}
