// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <math.h>
#include <stdint.h>

#include "samconf/samconf.h"
#include "samconfConfigGetRealOr_utest.h"

int samconfTestSamconfConfigGetRealOrSuccessNodeNotRealSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealOrSuccessNodeNotRealTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetRealOrSuccessNodeNotReal(void **state) {
    samconfConfig_t *testConfig = *state;
    double result = NAN;
    double defaultValues[] = {0.0, -12.3, DBL_MAX, DBL_MIN, 42.7, DBL_EPSILON, DBL_MAX_EXP, DBL_MIN_EXP};

    const char *testPath = "root/elos/ClientInputs/PluginSearchPath";

    TEST("samconfConfigGetRealOr");
    SHOULD("%s", "return the providet default value because no int32 was found");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        result = samconfConfigGetRealOr(testConfig, testPath, defaultValues[i]);
        assert_double_equal(result, defaultValues[i], DBL_EPSILON);
    }
}
