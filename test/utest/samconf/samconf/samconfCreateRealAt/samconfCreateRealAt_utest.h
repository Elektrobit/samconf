// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateRealAtSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateRealAtErrorNullPath)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateRealAtErrorNullRoot)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateRealAtExtErrorConfigNew)
