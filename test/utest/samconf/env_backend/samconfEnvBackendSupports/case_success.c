// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendSupports_utest.h"

int samconfTestSamconfEnvBackendSupportsSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendSupportsSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendSupportsSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "env://SAMCONF_UTEST_ENV";
    bool isSupported;

    TEST("samconfEnvBackendSupports");
    SHOULD("%s", "supports given backend as location is valid");

    result = samconfEnvBackendSupports(testLocation, &isSupported);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_true(isSupported);
}
