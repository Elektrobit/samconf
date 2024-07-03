// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "samconf/samconf.h"
#include "samconfConfigGetStringOr_utest.h"

int samconfTestSamconfConfigGetStringOrSuccessNodeNotStringSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringOrSuccessNodeNotStringTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringOrSuccessNodeNotString(void **state) {
    samconfConfig_t *testConfig = *state;
    const char *result;
    const char *defaultValues[] = {
        "",
        "foo",
    };

    const char *testPath = "root/elos/ClientInputs/Plugins/LocalTcpClient/Config/Port";

    TEST("samconfConfigGetStringOr");
    SHOULD("%s", "return the providet default value because no int32 was found");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetStringOr(testConfig, testPath, defaultValues[i]);
        assert_string_equal(result, defaultValues[i]);
    }
}
