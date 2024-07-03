// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBoolOr_utest.h"

int samconfTestSamconfConfigGetBoolOrSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolOrSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolOrSuccessEntryFound(void **state) {
    samconfConfig_t *testConfig = *state;
    bool resultBool;
    bool defaultBool;

    const char *testPath = "root/elos/UseEnv";

    TEST("samconfConfigGetBoolOr");
    SHOULD("%s", "return the correct value from the config");

    defaultBool = true;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_false(resultBool);

    defaultBool = false;
    resultBool = samconfConfigGetBoolOr(testConfig, testPath, defaultBool);
    assert_false(resultBool);
}
