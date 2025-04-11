# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    test_samconfGetParentPath_utest
  SOURCES
    case_env_success.c
    case_err_param_config.c
    case_err_param_path.c
    case_json_success.c
    samconfGetParentPath_utest.c
  LIBRARIES
    samconf_test_utils
)
