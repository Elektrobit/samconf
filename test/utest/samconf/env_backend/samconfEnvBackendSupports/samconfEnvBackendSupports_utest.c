// SPDX-License-Identifier: MIT
#include "samconfEnvBackendSupports_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfEnvBackendSupportsUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfEnvBackendSupportsSuccess),
        TEST_CASE(samconfTestSamconfEnvBackendSupportsErrorInvalidLocation),
        TEST_CASE(samconfTestSamconfEnvBackendSupportsExtErrorStrndup),
    };

    return RUN_TEST_SUITE(tests, samconfEnvBackendSupportsUtest);
}

static int samconfEnvBackendSupportsUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfEnvBackendSupportsUtestTeardown(UNUSED void **state) {
    return 0;
}
