// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

typedef struct {
    samconfConfig_t *testConfig;
} samconfUteststateT_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrSuccessEntryFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrSuccessEntryNotFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrSuccessNodeNotInt32)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrExtErrConfigGet)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrErrorRootNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32OrErrorPathNull)