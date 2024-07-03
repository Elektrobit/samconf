// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <stdint.h>

#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrErrorPathNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrErrorPathNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringOrErrorPathNull(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *defaultValues[] = {
        "",
        "foo",
    };
    const char *testPath = NULL;

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the provided default value since input path is null");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetStringOr(testConfig, testPath, defaultValues[i]);
        assert_string_equal(result, defaultValues[i]);
    }
}
