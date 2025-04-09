// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfGetParentPath_utest.h"

int samconfTestSamconfGetParentPathErrParamPathSetup(UNUSED void **state) {
    setenv("UTEST_VARIABLE_STRING", "utest string", 1);
    setenv("UTEST_VARIABLE_INT", "42", 1);
    setenv("UTEST_VARIABLE_REAL", "1.9865", 1);
    setenv("UTEST_VARIABLE_BOOL", "true", 1);
    return 0;
}

int samconfTestSamconfGetParentPathErrParamPathTeardown(UNUSED void **state) {
    unsetenv("UTEST_VARIABLE_STRING");
    unsetenv("UTEST_VARIABLE_INT");
    unsetenv("UTEST_VARIABLE_REAL");
    unsetenv("UTEST_VARIABLE_BOOL");
    return 0;
}

void samconfTestSamconfGetParentPathErrParamPath(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testroot = NULL;
    const samconfConfig_t *testconfig = NULL;
    const char *correctpath = "UTEST/VARIABLE/REAL";
    const char *testlocation = "env://?envPrefix=UTEST_VARIABLE&testVar=Check";

    TEST("samconfGetParentPath");
    SHOULD("%s", "not return expected path to given env config since null path is passed");

    result = samconfLoad(testlocation, false, &testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testroot, correctpath, &testconfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfGetParentPath(testconfig, NULL);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);

    result = samconfConfigDelete(testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
