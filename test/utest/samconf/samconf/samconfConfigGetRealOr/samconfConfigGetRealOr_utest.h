// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

typedef struct {
    samconfConfig_t *testConfig;
} samconfUteststateT_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrSuccessEntryFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrSuccessEntryNotFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrSuccessNodeNotReal)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrExtErrConfigGet)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrErrorRootNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetRealOrErrorPathNull)
