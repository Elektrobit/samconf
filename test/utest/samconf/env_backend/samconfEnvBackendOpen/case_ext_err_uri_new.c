// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfEnvBackendOpen_utest.h"

int samconfTestSamconfEnvBackendOpenExtErrorUriNewSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfEnvBackendOpenExtErrorUriNewTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfEnvBackendOpenExtErrorUriNew(UNUSED void **state) {
    samconfConfigStatusE_t result;
    const char *testLocation = "env://SAMCONF_UTEST_ENV";
    samconfConfigBackend_t testBackend = {
        .originalHandle = NULL,
        .backendHandle = NULL,
    };

    TEST("samconfEnvBackendOpen");
    SHOULD("%s", "not return expected uri object since uri create fails");

    MOCK_FUNC_AFTER_CALL(samconfUriNew, 0);
    expect_any(samconfUriNew, uri);
    expect_string(samconfUriNew, uriString, testLocation);
    will_return(samconfUriNew, NULL);
    will_return(samconfUriNew, SAMCONF_CONFIG_ERROR);

    result = samconfEnvBackendOpen(testLocation, &testBackend);
    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
