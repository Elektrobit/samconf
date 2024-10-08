// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "safu/common.h"
#include "samconf/samconf_types.h"
#include "samconf/test_utils.h"
#include "samconfGetParentPath_utest.h"

int samconfTestSamconfGetParentPathJsonSuccessSetup(void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testroot = calloc(1, sizeof(samconfConfig_t));
    result = samconfUtilCreateMockConfigFromStr(_TEST_CONFIG, false, testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    *state = testroot;
    return 0;
}

int samconfTestSamconfGetParentPathJsonSuccessTeardown(void **state) {
    samconfConfigStatusE_t result;
    samconfConfig_t *testroot = *(samconfConfig_t **)state;
    result = samconfConfigDelete(testroot);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    return 0;
}

void samconfTestSamconfGetParentPathJsonSuccess(void **state) {
    samconfConfig_t *testroot = *state;
    samconfConfigStatusE_t result;
    const samconfConfig_t *testconfig = NULL;
    const char *testpath = NULL;
    const char *correctpath = "root/elos/ClientInputs/Plugins/LocalTcpClient/File";

    TEST("samconfGetParentPath");
    SHOULD("%s", "returns expected path to given json config");

    result = samconfConfigGet(testroot, correctpath, &testconfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfGetParentPath(testconfig, &testpath);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(testpath, correctpath);

    free((char *)testpath);
}