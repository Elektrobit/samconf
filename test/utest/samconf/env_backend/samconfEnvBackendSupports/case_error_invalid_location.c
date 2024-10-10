// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendSupports_utest.h"

int samconfTestSamconfEnvBackendSupportsErrorInvalidLocationSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendSupportsErrorInvalidLocationTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendSupportsErrorInvalidLocation(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "xyz://SAMCONF_UTEST_ENV";
    bool isSupported = false;

    TEST("samconfEnvBackendSupports");
    SHOULD("%s", "not support given backend as location is invalid");

    result = samconfEnvBackendSupports(testLocation, &isSupported);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_false(isSupported);
}
