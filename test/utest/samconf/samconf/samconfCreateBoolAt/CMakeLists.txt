# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(cmocka_extensions 0.53.1 REQUIRED)
find_package(json-c REQUIRED)

create_unit_test(
  NAME
    test_samconfCreateBoolAt_utest
  SOURCES
    case_error_null_path.c
    case_error_null_root.c
    case_ext_error_config_new.c
    case_success.c
    samconfCreateBoolAt_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
