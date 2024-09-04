// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfEnvBackendOpen_utest.h"

int samconfTestSamconfEnvBackendOpenErrorInvalidLocationSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendOpenErrorInvalidLocationTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendOpenErrorInvalidLocation(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "ftp://SAMCONF_UTEST_ENV";
    samconfConfigBackend_t testBackend = {
        .originalHandle = NULL,
        .backendHandle = NULL,
    };

    TEST("samconfEnvBackendOpen");
    SHOULD("%s", "not return uri object since location is invalid");

    result = samconfEnvBackendOpen(testLocation, &testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
