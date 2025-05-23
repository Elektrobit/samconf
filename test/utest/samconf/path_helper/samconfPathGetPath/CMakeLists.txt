# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_samconfPathGetPath_utest
  SOURCES
    case_env_success.c
    case_err_param_config.c
    case_err_param_path.c
    case_json_success.c
    samconfPathGetPath_utest.c
  LIBRARIES
    samconf_static
    samconf_test_utils
)
