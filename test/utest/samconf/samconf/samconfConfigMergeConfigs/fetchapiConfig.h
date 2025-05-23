// SPDX-License-Identifier: MIT
#pragma once

#define _TEST_CONFIG_FETCHAPI \
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
                         }\
                    }\
                }\
            }\
        }\
     }"
