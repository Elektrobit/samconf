// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>

#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32OrErrorRootNull(UNUSED void **state) {
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the provided default value since no config was provided");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetInt32Or(root, testPath, defaultValues[i]);
        assert_int_equal(result, defaultValues[i]);
    }
}
