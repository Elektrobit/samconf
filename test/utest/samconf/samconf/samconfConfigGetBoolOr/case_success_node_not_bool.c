// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "samconf/samconf.h"
#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrSuccessNodeNotBoolSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrSuccessNodeNotBoolTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrSuccessNodeNotBool(void **state) {
    samconfConfig_t *testConfig = *state;
    bool resultBool;
    bool defaultBool;

    const char *testPath = "root/elos/ClientInputs/PluginSearchPath";

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the providet default value because no bool was found");

    defaultBool = false;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_int_equal(resultBool, defaultBool);
    assert_false(resultBool);

    defaultBool = true;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_int_equal(resultBool, defaultBool);
    assert_true(resultBool);
}
