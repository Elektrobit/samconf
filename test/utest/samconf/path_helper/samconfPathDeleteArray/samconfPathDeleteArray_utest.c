// SPDX-License-Identifier: MIT

#include "samconfPathDeleteArray_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfPathDeleteArrayUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfPathDeleteArraySuccess),
    };

    return RUN_TEST_SUITE(tests, samconfPathDeleteArrayUtest);
}

static int samconfPathDeleteArrayUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfPathDeleteArrayUtestTeardown(UNUSED void **state) {
    return 0;
}
