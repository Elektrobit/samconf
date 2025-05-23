// SPDX-License-Identifier: MIT
#pragma once

#include "samconf_utest.h"

#define _TEST_CONFIG \
    "{\
        \"root\": {\
            \"elos\": {\
                \"EventLogging\": {\
                    \"Plugins\": {\
                        \"fetchapi\": {\
                            \"File\": \"backend_fetchapi.so\",\
                            \"Run\": \"always\",\
                            \"Filter\": [\
                                \"1 1 EQ\"\
                            ],\
                            \"Config\": {\
                                \"BufferSize\": 100\
                             }\
                         },\
                        \"JsonBackend\": {\
                            \"File\": \"backend_json.so\",\
                            \"Run\": \"always\",\
                            \"Filter\": [\
                                \"1 1 EQ\"\
                            ],\
                            \"Config\": {\
                                \"StoragePath\": \"/tmp/elosd_%host%_%date%_%count%.log\",\
                                \"MaxSize\": 60000,\
                                \"Flags\": [\
                                    \"O_SYNC\"\
                                ]\
                            }\
                         },\
                        \"SQLBackend\": {\
                            \"File\": \"backend_sql.so\",\
                            \"Run\": \"always\",\
                            \"Filter\": [\
                                \"1 1 EQ\"\
                            ],\
                            \"Config\": {\
                                \"ConnectionPath\": \"/tmp/elos.sqlite\"\
                            }\
                         },\
                        \"DLT\": {\
                            \"File\": \"backend_dlt_logger.so\",\
                            \"Run\": \"always\",\
                            \"Filter\": [\
                                \".e.messageCode 1000 GE\"\
                            ],\
                            \"Config\": {\
                                \"Connection\": \"/tmp/dlt\",\
                                \"EcuId\": \"ECU1\",\
                                \"AppId\": \"ELOS\"\
                            }\
                         }\
                    }\
                }\
            }\
        }\
     }"
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigNextSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigNextError)
