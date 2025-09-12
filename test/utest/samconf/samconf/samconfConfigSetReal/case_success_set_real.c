// SPDX-License-Identifier: MIT
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "samconfConfigSetReal_utest.h"

int samconfTestSamconfConfigSetRealSuccessSetup(void **state) {
    samconfConfig_t *root = NULL;
    samconfConfigNew(&root);
    root->key = strdup("node");
    root->type = SAMCONF_CONFIG_VALUE_STRING;
    root->value.string = strdup("test");
    *state = root;
    return 0;
}

int samconfTestSamconfConfigSetRealSuccessTeardown(void **state) {
    samconfConfig_t *root = *state;
    samconfConfigDelete(root);
    return 0;
}

void samconfTestSamconfConfigSetRealSuccess(void **state) {
    samconfConfig_t *root = *state;
    double testValue[] = {6.2898, DBL_MAX, DBL_MIN, 0.0, 3.143};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    TEST("samconfConfigSetReal");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given real value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetReal(root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root->type, SAMCONF_CONFIG_VALUE_REAL);
        assert_float_equal(root->value.real, testValue[i], 0.0);
    }
}
