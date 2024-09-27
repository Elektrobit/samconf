// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendClose_utest.h"

int samconfTestSamconfEnvBackendCloseSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendCloseSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendCloseSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "env://SAMCONF_UTEST_ENV";
    samconfConfigBackend_t testBackend = {
        .originalHandle = NULL,
        .backendHandle = NULL,
    };

    TEST("samconfEnvBackendClose");
    SHOULD("%s", "delete existing backend struct");

    result = samconfEnvBackendOpen(testLocation, &testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfEnvBackendClose(&testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
