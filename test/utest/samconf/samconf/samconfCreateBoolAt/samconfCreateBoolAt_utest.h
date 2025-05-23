// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateBoolAtSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateBoolAtErrorNullPath)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateBoolAtErrorNullRoot)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateBoolAtExtErrorConfigNew)
