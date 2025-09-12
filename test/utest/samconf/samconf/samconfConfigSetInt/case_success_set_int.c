// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <string.h>

#include "samconf/samconf.h"
#include "samconf/samconf_types.h"
#include "samconfConfigSetInt_utest.h"

int samconfTestSamconfConfigSetIntSuccessSetup(void **state) {
    samconfConfig_t *root = NULL;
    samconfConfigNew(&root);
    root->key = strdup("node");
    root->type = SAMCONF_CONFIG_VALUE_STRING;
    root->value.string = strdup("test");
    *state = root;
    return 0;
}

int samconfTestSamconfConfigSetIntSuccessTeardown(void **state) {
    samconfConfig_t *root = *state;
    samconfConfigDelete(root);
    return 0;
}

void samconfTestSamconfConfigSetIntSuccess(void **state) {
    samconfConfig_t *root = *state;
    int64_t testValue[] = {0x7F102B892386AF1F, INT64_MAX, 0, INT64_MIN};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    TEST("samconfConfigSetInt");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given integer value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetInt(root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root->type, SAMCONF_CONFIG_VALUE_INT);
        assert_int_equal(root->value.integer, testValue[i]);
    }
}
