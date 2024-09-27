// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "samconfUriGetParameter_utest.h"

int samconfTestSamconfUriGetParameterErrParamSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriGetParameterErrParamTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfUriGetParameterErrParam(UNUSED void **state) {
    samconfConfigStatusE_t result;
    samconfUri_t *uri;
    const char *uriParam = "Prefix";
    char *filter = NULL;

    const char *testUris[] = {"env://?prefixEnv=UTEST_VARIABLE&testVar1=Val1&testVar2=Val2",
                              "env://?testVar1=Val1&prefix=UTEST_VARIABLE&testVar2=Val2",
                              "env://?testVar1=Val1&envprefix=UTEST_VARIABLE&testVar2=Val2"};

    TEST("samconfUriGetParameter");
    SHOULD("%s", " not return value of invalid parameter");

    for (size_t i = 0; i < ARRAY_SIZE(testUris); i++) {
        PARAM("URI : %s", testUris[i]);
        result = samconfUriNew(&uri, testUris[i]);
        assert_int_equal(result, SAMCONF_CONFIG_OK);

        result = samconfUriGetParameter(uri, uriParam, &filter);
        assert_int_equal(result, SAMCONF_CONFIG_ERROR);

        result = samconfUriDelete(uri);
        assert_int_equal(result, SAMCONF_CONFIG_OK);

        free(filter);
    }
}
