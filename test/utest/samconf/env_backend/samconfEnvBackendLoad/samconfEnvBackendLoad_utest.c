// SPDX-License-Identifier: MIT
#include "samconfEnvBackendLoad_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfEnvBackendLoadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfEnvBackendLoadSuccess),
        TEST_CASE(samconfTestSamconfEnvBackendLoadError),
        TEST_CASE(samconfTestSamconfEnvBackendLoadExtErrorCreateConfig),
    };

    return RUN_TEST_SUITE(tests, samconfEnvBackendLoadUtest);
}

static int samconfEnvBackendLoadUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfEnvBackendLoadUtestTeardown(UNUSED void **state) {
    return 0;
}
