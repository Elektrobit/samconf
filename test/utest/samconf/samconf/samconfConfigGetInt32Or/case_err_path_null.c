// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <stdint.h>

#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrErrorPathNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrErrorPathNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32OrErrorPathNull(void **state) {
    samconfConfig_t *testConfig = *state;
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};
    const char *testPath = NULL;

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the provided default value since input path is null");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetInt32Or(testConfig, testPath, defaultValues[i]);
        assert_int_equal(result, defaultValues[i]);
    }
}
