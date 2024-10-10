// SPDX-License-Identifier: MIT
#include "samconfEnvBackendOpen_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfEnvBackendOpenUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfEnvBackendOpenSuccess),
        TEST_CASE(samconfTestSamconfEnvBackendOpenErrorInvalidLocation),
        TEST_CASE(samconfTestSamconfEnvBackendOpenExtErrorUriNew),
    };

    return RUN_TEST_SUITE(tests, samconfEnvBackendOpenUtest);
}

static int samconfEnvBackendOpenUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfEnvBackendOpenUtestTeardown(UNUSED void **state) {
    return 0;
}
