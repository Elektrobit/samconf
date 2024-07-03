// SPDX-License-Identifier: MIT
#include "samconfConfigGetInt32Or_utest.h"

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

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetInt32OrUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigGetInt32OrSuccessEntryFound),
        TEST_CASE(samconfTestSamconfConfigGetInt32OrSuccessEntryNotFound),
        TEST_CASE(samconfTestSamconfConfigGetInt32OrSuccessNodeNotInt32),
        TEST_CASE(samconfTestSamconfConfigGetInt32OrExtErrConfigGet),
        TEST_CASE(samconfTestSamconfConfigGetInt32OrErrorRootNull),
        TEST_CASE(samconfTestSamconfConfigGetInt32OrErrorPathNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigGetInt32OrUtest);
}

static int samconfConfigGetInt32OrUtestSetup(UNUSED void **state) {
    samconfConfig_t *testConfig = calloc(1, sizeof(samconfConfig_t));
    samconfUtilCreateMockConfigFromStr(_TEST_CONFIG, false, testConfig);
    *state = testConfig;
    return 0;
}

static int samconfConfigGetInt32OrUtestTeardown(UNUSED void **state) {
    samconfConfig_t *testConfig = *(samconfConfig_t **)state;
    samconfConfigDelete(testConfig);
    state = NULL;
    return 0;
}
