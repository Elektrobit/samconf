// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/env_backend.h"
#include "samconf/samconf.h"
#include "samconf/uri.h"

typedef struct samconfEnvBackendLoadTestData {
    samconfConfigBackend_t *backend;
    samconfConfig_t *config;
} samconfEnvBackendLoadTestData_t;

typedef struct samconfEnvBackendTestValues {
    char *string;
    int64_t integer;
    double real;
    bool boolean;
} samconfEnvBackendTestValues_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendLoadSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendLoadError)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendLoadExtErrorCreateConfig)
