// SPDX-License-Identifier: MIT
#ifndef SAMCONF_CONFIGGETINT32_UTEST_H
#define SAMCONF_CONFIGGETINT32_UTEST_H

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

typedef struct {
    samconfConfig_t *childrenData;
} samconfUteststateT_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32SuccessEntryFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32SuccessEntryNotFound)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32SuccessNodeNotInt32)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32ExtErrConfigGet)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32ErrorRootNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32ErrorPathNull)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigGetInt32ErrorResultNull)
#endif /* SAMCONF_CONFIGGETINT32_UTEST_H */