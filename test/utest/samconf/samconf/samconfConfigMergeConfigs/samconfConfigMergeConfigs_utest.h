// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>

#include "defConfig.h"
#include "dltConfig.h"
#include "fetchapiConfig.h"
#include "jsonConfig.h"
#include "samconf/test_utils.h"
#include "sqlConfig.h"

typedef struct {
    samconfConfig_t *defConfig;
    samconfConfig_t *fetchapiConfig;
    samconfConfig_t *jsonConfig;
    samconfConfig_t *sqlConfig;
    samconfConfig_t *dltConfig;
} samconfTestConfigs_t;

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigMergeConfigsSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigMergeConfigsSignageSuccess)
