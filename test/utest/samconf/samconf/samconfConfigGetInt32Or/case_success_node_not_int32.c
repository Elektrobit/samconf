// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "samconf/samconf.h"
#include "samconfConfigGetInt32Or_utest.h"

int samconfTestSamconfConfigGetInt32OrSuccessNodeNotInt32Setup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32OrSuccessNodeNotInt32Teardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32OrSuccessNodeNotInt32(void **state) {
    samconfConfig_t *testConfig = *state;
    int32_t result;
    int32_t defaultValues[] = {0, -12, INT32_MAX, INT32_MIN, 42};

    const char *testPath = "root/elos/ClientInputs/PluginSearchPath";

    TEST("samconfConfigGetInt32Or");
    SHOULD("%s", "return the providet default value because no int32 was found");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetInt32Or(testConfig, testPath, defaultValues[i]);
        assert_int_equal(result, defaultValues[i]);
    }
}
