# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(cmocka_extensions 0.53.1 REQUIRED)
find_package(json-c REQUIRED)

create_unit_test(
  NAME
    test_samconfEnvBackendLoad_utest
  SOURCES
    case_error.c
    case_exterr_create_config.c
    case_success.c
    samconfEnvBackendLoad_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
