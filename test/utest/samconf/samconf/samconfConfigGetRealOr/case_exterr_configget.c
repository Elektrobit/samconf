// SPDX-License-Identifier: MIT
#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetRealOr_utest.h"

int samconfTestSamconfConfigGetRealOrExtErrConfigGetSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealOrExtErrConfigGetTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetRealOrExtErrConfigGet(void **state) {
    samconfConfig_t *testConfig = *state;
    double result = NAN;
    double defaultValues[] = {0.0, -12.3, DBL_MAX, DBL_MIN, 42.7, DBL_EPSILON, DBL_MAX_EXP, DBL_MIN_EXP};

    const char *testPath = "Scanner/Syslog/SyslogPath";

    TEST("samconfConfigGetRealOr");
    SHOULD("%s",
           "return the providet default value after samconfConfigGet "
           "internaly failed");

    for (size_t i = 0; i < ARRAY_SIZE(defaultValues); i++) {
        MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
        expect_value(samconfConfigGet, root, testConfig);
        expect_value(samconfConfigGet, path, testPath);
        expect_any(samconfConfigGet, result);

        will_return(samconfConfigGet, NULL);
        will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

        result = samconfConfigGetRealOr(testConfig, testPath, defaultValues[i]);
        assert_double_equal(result, defaultValues[i], DBL_EPSILON);
    }
}
