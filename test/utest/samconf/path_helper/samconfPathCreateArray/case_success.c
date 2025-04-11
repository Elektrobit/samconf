// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "samconfPathCreateArray_utest.h"

int samconfTestSamconfPathCreateArraySuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfPathCreateArraySuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfPathCreateArraySuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;
    const char *testPaths[] = {"Scanner/Syslog/SyslogPath",
                               "Scanner/Syslog",
                               "Scanner",
                               "logger",
                               "ip",
                               "/Scanner/Syslog/SyslogPath",
                               "Scanner/",
                               "/Scanner/",
                               "/Scanner",
                               "/Scanner/Syslog/SyslogPath/",
                               "Scanner/Syslog/SyslogPath/",
                               "/////Scanner///Syslog///SyslogPath///",
                               "A",
                               "",
                               "/",
                               "//",
                               "///"};

    const char *segments[][3] = {{"Scanner", "Syslog", "SyslogPath"},
                                 {"Scanner", "Syslog"},
                                 {"Scanner"},
                                 {"logger"},
                                 {"ip"},
                                 {"Scanner", "Syslog", "SyslogPath"},
                                 {"Scanner"},
                                 {"Scanner"},
                                 {"Scanner"},
                                 {"Scanner", "Syslog", "SyslogPath"},
                                 {"Scanner", "Syslog", "SyslogPath"},
                                 {"Scanner", "Syslog", "SyslogPath"},
                                 {"A"},
                                 {""},
                                 {""},
                                 {""},
                                 {""}};
    int actualcount[ARRAY_SIZE(testPaths)] = {3, 2, 1, 1, 1, 3, 1, 1, 1, 3, 3, 3, 1, 0, 0, 0, 0};
    char **testarray = NULL;
    int resultcount;

    TEST("samconfPathCreateArray");
    SHOULD("%s", "return array from given path string");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        PARAM("Path %s", testPaths[i]);
        status = samconfPathCreateArray(testPaths[i], &testarray, &resultcount);

        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(resultcount, actualcount[i]);

        for (int j = 0; j < resultcount; j++) {
            assert_string_equal(testarray[j], segments[i][j]);
        }
        samconfPathDeleteArray(testarray, resultcount);
    }
}
