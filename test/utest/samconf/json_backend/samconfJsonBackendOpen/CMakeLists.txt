# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(json-c REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_samconfJsonBackendOpen_utest
  SOURCES
    case_success_file.c
    case_error_file.c
    samconfJsonBackendOpen_utest.c
  LIBRARIES
    samconf_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
)
