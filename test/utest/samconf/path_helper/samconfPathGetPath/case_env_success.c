// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfPathGetPath_utest.h"

int samconfTestSamconfPathGetPathEnvSuccessSetup(void **state) {
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

int samconfTestSamconfPathGetPathEnvSuccessTeardown(void **state) {
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

void samconfTestSamconfPathGetPathEnvSuccess(void **state) {
    samconfConfig_t *testroot = *state;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testconfig = NULL;
    const char *testpath = NULL;
    const char *conflocation = "UTEST/VARIABLE/REAL";

    TEST("samconfPathGetPath");
    SHOULD("%s", "returns expected path to given env config");

    result = samconfConfigGet(testroot, conflocation, &testconfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfPathGetPath(testconfig, &testpath);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testpath, "root/UTEST/VARIABLE/REAL");

    free((char *)testpath);
}
