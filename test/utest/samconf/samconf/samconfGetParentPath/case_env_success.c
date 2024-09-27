// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfGetParentPath_utest.h"

int samconfTestSamconfGetParentPathEnvSuccessSetup(void **state) {
    setenv("UTEST_VARIABLE_STRING", "utest string", 1);
    setenv("UTEST_VARIABLE_INT", "42", 1);
    setenv("UTEST_VARIABLE_REAL", "1.9865", 1);
    setenv("UTEST_VARIABLE_BOOL", "true", 1);

    samconfConfigStatusE_t result;
    samconfConfig_t *testroot = NULL;

    const char *testlocation = "env://?envPrefix=UTEST_VARIABLE&testVar=Check";

    result = samconfLoad(testlocation, false, &testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    *state = testroot;
    return 0;
}

int samconfTestSamconfGetParentPathEnvSuccessTeardown(void **state) {
    unsetenv("UTEST_VARIABLE_STRING");
    unsetenv("UTEST_VARIABLE_INT");
    unsetenv("UTEST_VARIABLE_REAL");
    unsetenv("UTEST_VARIABLE_BOOL");

    samconfConfig_t *testroot = *(samconfConfig_t **)state;
    samconfConfigStatusE_t result;
    result = samconfConfigDelete(testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    return 0;
}

void samconfTestSamconfGetParentPathEnvSuccess(void **state) {
    samconfConfig_t *testroot = *state;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testconfig = NULL;
    const char *testpath = NULL;
    const char *correctpath = "root/UTEST/VARIABLE/REAL";

    TEST("samconfGetParentPath");
    SHOULD("%s", "returns expected path to given env config");

    result = samconfConfigGet(testroot, correctpath, &testconfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfGetParentPath(testconfig, &testpath);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testpath, correctpath);

    free((char *)testpath);
}
