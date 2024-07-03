// SPDX-License-Identifier: MIT
#include "samconfConfigGetStringOr_utest.h"

#include "safu/common.h"
#include "samconf/samconf_types.h"
#include "samconf/test_utils.h"

#define _TEST_CONFIG \
    "{\
        \"root\": {\
            \"elos\": {\
                \"UseEnv\": false,\
                \"ClientInputs\": {\
                    \"PluginSearchPath\": \"build/Debug/cmake/src/plugins/clients/dummy/client_dummy.so\",\
                    \"Plugins\": {\
                        \"LocalTcpClient\": {\
                            \"File\": \"client_dummy.so\",\
                            \"Run\": \"always\",\
                            \"Config\": {\
                                \"Port\": 54321,\
                                \"Interface\": \"127.0.0.1\",\
                                \"EventBlacklist\": \".event.messageCode 2000 EQ\",\
                                \"authorizedProcesses\": [\
                                    \".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                    \".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                    \".process.pid 1 EQ\"\
                                ]\
                            }\
                        },\
                        \"PublicTcpClient\": {\
                            \"File\": \"client_dummy.so\",\
                            \"Run\": \"always\",\
                            \"Config\": {\
                                \"Port\": 5422,\
                                \"Interface\": \"0.0.0.0\",\
                                \"EventBlacklist\": \".event.messageCode 2000 EQ\",\
                                \"authorizedProcesses\": []\
                            }\
                        }\
                    }\
                }\
            }\
        }\
    }"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetStringOrUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigGetStringOrSuccessEntryFound),
        TEST_CASE(samconfTestSamconfConfigGetStringOrSuccessEntryNotFound),
        TEST_CASE(samconfTestSamconfConfigGetStringOrSuccessNodeNotString),
        TEST_CASE(samconfTestSamconfConfigGetStringOrExtErrConfigGet),
        TEST_CASE(samconfTestSamconfConfigGetStringOrErrorRootNull),
        TEST_CASE(samconfTestSamconfConfigGetStringOrErrorPathNull),
        TEST_CASE(samconfTestSamconfConfigGetStringOrSuccessDefaultNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigGetStringOrUtest);
}

static int samconfConfigGetStringOrUtestSetup(UNUSED void **state) {
    samconfConfig_t *testConfig = calloc(1, sizeof(samconfConfig_t));
    samconfUtilCreateMockConfigFromStr(_TEST_CONFIG, false, testConfig);
    *state = testConfig;
    return 0;
}

static int samconfConfigGetStringOrUtestTeardown(UNUSED void **state) {
    samconfConfig_t *testConfig = *(samconfConfig_t **)state;
    samconfConfigDelete(testConfig);
    state = NULL;
    return 0;
}
