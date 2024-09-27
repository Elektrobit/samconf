// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

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

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfGetParentPathEnvSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfGetParentPathJsonSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfGetParentPathErrParamConfig)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfGetParentPathErrParamPath)
