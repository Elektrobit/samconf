// SPDX-License-Identifier: MIT
#include <float.h>
#include <math.h>
#include <stddef.h>

#include "samconfConfigGetRealOr_utest.h"

int samconfTestSamconfConfigGetRealOrErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealOrErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetRealOrErrorRootNull(UNUSED void **state) {
    double result = NAN;
    double defaultValues[] = {0.0, -12.3, DBL_MAX, DBL_MIN, 42.7, DBL_EPSILON, DBL_MAX_EXP, DBL_MIN_EXP};
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetRealOr");
    SHOULD("%s", "return the provided default value since no config was provided");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetRealOr(root, testPath, defaultValues[i]);
        assert_double_equal(result, defaultValues[i], DBL_EPSILON);
    }
}
