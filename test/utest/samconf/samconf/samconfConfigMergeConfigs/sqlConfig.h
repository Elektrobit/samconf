// SPDX-License-Identifier: MIT
#pragma once

#define _TEST_CONFIG_SQL \
    "{\
        \"root\": {\
            \"elos\": {\
                \"EventLogging\": {\
                    \"Plugins\": {\
                        \"SQLBackend\": {\
                            \"File\": \"backend_sql.so\",\
                            \"Run\": \"always\",\
                            \"Filter\": [\
                                \"1 1 EQ\"\
                            ],\
                            \"Config\": {\
                                \"ConnectionPath\": \"/tmp/elos.sqlite\"\
                            }\
                         }\
                    }\
                }\
            }\
        }\
     }"
