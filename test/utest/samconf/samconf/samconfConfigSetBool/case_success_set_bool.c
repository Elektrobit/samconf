// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <string.h>

#include "samconfConfigSetBool_utest.h"

int samconfTestSamconfConfigSetBoolSuccessSetup(void **state) {
    samconfConfig_t *root = NULL;
    samconfConfigNew(&root);
    root->key = strdup("node");
    root->type = SAMCONF_CONFIG_VALUE_STRING;
    root->value.string = strdup("test");
    *state = root;
    return 0;
}

int samconfTestSamconfConfigSetBoolSuccessTeardown(void **state) {
    samconfConfig_t *root = *state;
    samconfConfigDelete(root);
    return 0;
}

void samconfTestSamconfConfigSetBoolSuccess(void **state) {
    samconfConfig_t *root = *state;
    bool testValue[] = {true, false};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    TEST("samconfConfigSetBool");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given boolean value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetBool(root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root->type, SAMCONF_CONFIG_VALUE_BOOLEAN);
        assert_int_equal(root->value.boolean, testValue[i]);
    }
}
