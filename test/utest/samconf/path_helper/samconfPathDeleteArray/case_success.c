// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "samconfPathDeleteArray_utest.h"

int samconfTestSamconfPathDeleteArraySuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfPathDeleteArraySuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfPathDeleteArraySuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    const char *testPaths[] = {
        "Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",   "logger",   "ip",
        "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/", "/Scanner", "/Scanner/Syslog/SyslogPath/",
        "Scanner/Syslog/SyslogPath/"};

    char **testarray = NULL;
    int resultcount;

    TEST("samconfPathDeleteArray");
    SHOULD("%s", "delete given path array");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        status = samconfPathCreateArray(testPaths[i], &testarray, &resultcount);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        samconfPathDeleteArray(testarray, resultcount);
    }
}
