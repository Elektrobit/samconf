// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"
#include "samconfGetBackendOps_utest.h"

void samconfTestSamconfGetBackendOpsError(UNUSED void **state) {
    const samconfConfigBackendOps_t *ops;

    TEST("samconfGetBackendOps with invalid input");
    SHOULD("%s", "return Null");

    ops = samconfGetBackendOps(3);

    assert_null(ops);
}
