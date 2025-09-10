// SPDX-License-Identifier: MIT

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_libc.h"
#include "samconf/samconf_types.h"
#include "samconfJsonBackendOpen_utest.h"

int samconfTestSamconfJsonBackendOpenFileErrFileNotFoundSetup(UNUSED void **state) {
    json_object *jobjDummy = malloc(sizeof(json_object *));
    *state = jobjDummy;
    return 0;
}

int samconfTestSamconfJsonBackendOpenFileErrFileNotFoundTeardown(UNUSED void **state) {
    __real_free(*state);
    return 0;
}

void samconfTestSamconfJsonBackendOpenFileErrFileNotFound(UNUSED void **state) {
    char path[] = "test.json";
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfigBackend_t backend = {
        .originalHandle = path,
        .backendHandle = NULL,
    };

    TEST("samconfJsonBackendOpen with existing File input");
    SHOULD("%s", "return expected json_object inside struct");

    MOCK_FUNC_AFTER_CALL(stat, 0);
    expect_string(__wrap_stat, pathname, path);
    expect_any(__wrap_stat, statbuf);
    will_return_and_set_errno(__wrap_stat, -1, ENOENT);

    result = samconfJsonBackendOpen(path, &backend);

    assert_int_equal(result, SAMCONF_CONFIG_NOT_FOUND);
    assert_string_equal(backend.originalHandle, path);
}
