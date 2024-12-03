// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "samconfPathGetPathUntil_utest.h"

int samconfTestSamconfPathGetPathUntilSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfPathGetPathUntilSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfPathGetPathUntilSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status;

    const char *testPath = "elos/ClientInputs/Plugins/LocalTcpClient/Config/Port";
    const char *expectedPaths[] = {"elos",
                                   "elos/ClientInputs",
                                   "elos/ClientInputs/Plugins",
                                   "elos/ClientInputs/Plugins/LocalTcpClient",
                                   "elos/ClientInputs/Plugins/LocalTcpClient/Config",
                                   "elos/ClientInputs/Plugins/LocalTcpClient/Config/Port"};

    char **pathArray = NULL;
    int pathArraySize;
    char *resultPath = NULL;

    TEST("samconfPathGetPathUntil");
    SHOULD("%s", "return path string till given segment in path");

    status = samconfPathCreateArray(testPath, &pathArray, &pathArraySize);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_int_equal(pathArraySize, 6);

    for (int i = 0; i < pathArraySize; i++) {
        status = samconfPathGetPathUntil(pathArray, i, &resultPath);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_string_equal(resultPath, expectedPaths[i]);

        free(resultPath);
    }
    samconfPathDeleteArray(pathArray, pathArraySize);
}
