// SPDX-License-Identifier: MIT
#include "samconfEnvBackendCreateConfig_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfEnvBackendCreateConfigUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigSuccess),
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigErrorNullPath),
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigErrorNullRoot),
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigErrorNullValue),
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigExtErrorConfigNew),
        TEST_CASE(samconfTestSamconfEnvBackendCreateConfigExtErrorConfigAdd),
    };

    return RUN_TEST_SUITE(tests, samconfEnvBackendCreateConfigUtest);
}

static int samconfEnvBackendCreateConfigUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfEnvBackendCreateConfigUtestTeardown(UNUSED void **state) {
    return 0;
}
