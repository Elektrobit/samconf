// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendOpen_utest.h"

int samconfTestSamconfEnvBackendOpenSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendOpenSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendOpenSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "env://SAMCONF_UTEST_ENV";
    samconfConfigBackend_t testBackend = {
        .originalHandle = NULL,
        .backendHandle = NULL,
    };

    TEST("samconfEnvBackendOpen");
    SHOULD("%s", "return expected uri object inside struct");

    result = samconfEnvBackendOpen(testLocation, &testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    result = samconfEnvBackendClose(&testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
