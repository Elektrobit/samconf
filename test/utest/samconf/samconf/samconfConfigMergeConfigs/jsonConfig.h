// SPDX-License-Identifier: MIT
#pragma once

#define _TEST_CONFIG_JSON \
    "{\
        \"root\": {\
            \"elos\": {\
                \"EventLogging\": {\
                    \"Plugins\": {\
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
                         }\
                    }\
                }\
            }\
        }\
     }"
