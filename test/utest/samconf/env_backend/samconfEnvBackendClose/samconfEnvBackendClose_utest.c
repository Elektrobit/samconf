// SPDX-License-Identifier: MIT
#include "samconfEnvBackendClose_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfEnvBackendCloseUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfEnvBackendCloseSuccess),
        TEST_CASE(samconfTestSamconfEnvBackendCloseErrParam),
    };

    return RUN_TEST_SUITE(tests, samconfEnvBackendCloseUtest);
}

static int samconfEnvBackendCloseUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfEnvBackendCloseUtestTeardown(UNUSED void **state) {
    return 0;
}
