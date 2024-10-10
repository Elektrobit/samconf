// SPDX-License-Identifier: MIT
#include "samconfUriGetParameter_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriGetParameterUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriGetParameterSuccess),
        TEST_CASE(samconfTestSamconfUriGetParameterErrParam),
        TEST_CASE(samconfTestSamconfUriGetParameterErrUri),
    };

    return RUN_TEST_SUITE(tests, samconfUriGetParameterUtest);
}

static int samconfUriGetParameterUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriGetParameterUtestTeardown(UNUSED void **state) {
    return 0;
}
