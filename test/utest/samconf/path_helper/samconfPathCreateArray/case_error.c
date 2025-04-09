// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <samconf/samconf_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "samconfPathCreateArray_utest.h"

int samconfTestSamconfPathCreateArrayErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfPathCreateArrayErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfPathCreateArrayError(UNUSED void **state) {
    samconfConfigStatusE_t status;
    char **testarray = NULL;
    int resultcount;
    TEST("samconfPathCreateArray");
    SHOULD("%s", "return error since path is NULL");

    status = samconfPathCreateArray(NULL, &testarray, &resultcount);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
