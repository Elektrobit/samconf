// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/env_backend.h"
#include "samconf/samconf.h"
#include "samconf/uri.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendOpenSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendOpenErrorInvalidLocation)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfEnvBackendOpenExtErrorUriNew)
