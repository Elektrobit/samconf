// SPDX-License-Identifier: MIT
#pragma once

#define _TEST_CONFIG_DLT \
    "{\
        \"root\": {\
            \"elos\": {\
                \"EventLogging\": {\
                    \"Plugins\": {\
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
