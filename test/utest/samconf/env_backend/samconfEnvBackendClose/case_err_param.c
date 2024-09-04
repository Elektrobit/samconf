// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendClose_utest.h"

int samconfTestSamconfEnvBackendCloseErrParamSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCloseErrParamTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCloseErrParam(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfConfigBackend_t *testBackend = NULL;

    TEST("samconfEnvBackendClose");
    SHOULD("%s", "not delete existing backend struct as struct is null");

    result = samconfEnvBackendClose(testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
