// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_libc.h"
#include "samconfEnvBackendSupports_utest.h"

int samconfTestSamconfEnvBackendSupportsExtErrorStrndupSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendSupportsExtErrorStrndupTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendSupportsExtErrorStrndup(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "env://SAMCONF_UTEST_ENV";
    bool isSupported = false;

    MOCK_FUNC_AFTER_CALL(strndup, 0);
    expect_string(__wrap_strndup, string, testLocation);
    expect_value(__wrap_strndup, n, 3);
    will_return(__wrap_strndup, NULL);

    TEST("samconfEnvBackendSupports");
    SHOULD("%s", "not support given backend as strndup fails");

    result = samconfEnvBackendSupports(testLocation, &isSupported);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_false(isSupported);
}
