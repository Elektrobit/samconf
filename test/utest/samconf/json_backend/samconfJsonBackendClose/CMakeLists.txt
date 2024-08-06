# SPDX-License-Identifier: MIT
find_package(safu REQUIRED)
find_package(json-c REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_samconfJsonBackendClose_utest
  SOURCES
    case_success.c
    case_failure.c
    samconfJsonBackendClose_utest.c
  LIBRARIES
    samconf_static
    safu::mock_safu
    cmocka_mocks::mock_jsonc
    cmocka_mocks::mock_libc
)
