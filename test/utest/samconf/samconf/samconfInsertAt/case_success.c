// SPDX-License-Identifier: MIT
#include <stdlib.h>
#include <string.h>

#include "samconfInsertAt_utest.h"

int samconfTestSamconfInsertAtSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfInsertAtSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfInsertAtSuccess(UNUSED void **state) {
    samconfConfig_t *testConfig = NULL;
    samconfConfig_t *testinsertnode = NULL;
    const samconfConfig_t *testgetnode = NULL;
    samconfConfigStatusE_t result;
    const char *testPath = "utest/utest/variable/node";
    const char *nodePath = NULL;

    TEST("samconfInsertAt");
    SHOULD("%s", "insert node in the given path");

    result = samconfConfigNew(&testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    testConfig->key = strdup("root");
    testConfig->type = SAMCONF_CONFIG_VALUE_OBJECT;

    result = samconfConfigNew(&testinsertnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    testinsertnode->key = strdup("node");
    testinsertnode->type = SAMCONF_CONFIG_VALUE_INT;
    testinsertnode->value.integer = 42;

    result = samconfInsertAt(&testConfig, testPath, testinsertnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfConfigGet(testConfig, "utest/utest/variable/node", &testgetnode);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    assert_string_equal(testgetnode->key, "node");
    assert_int_equal(testgetnode->type, SAMCONF_CONFIG_VALUE_INT);
    assert_int_equal(testgetnode->value.integer, 42);

    result = samconfPathGetPath(testgetnode, &nodePath);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(nodePath, "root/utest/utest/variable/node");

    result = samconfConfigDelete(testConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    free((char *)nodePath);
}
