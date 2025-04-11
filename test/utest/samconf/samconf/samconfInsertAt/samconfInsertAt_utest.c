// SPDX-License-Identifier: MIT
#include "samconfInsertAt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfInsertAtUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfInsertAtSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfInsertAtUtest);
}

static int samconfInsertAtUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfInsertAtUtestTeardown(UNUSED void **state) {
    return 0;
}
