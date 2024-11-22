// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateIntAtSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateIntAtErrorNullPath)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateIntAtErrorNullRoot)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfCreateIntAtExtErrorConfigNew)
