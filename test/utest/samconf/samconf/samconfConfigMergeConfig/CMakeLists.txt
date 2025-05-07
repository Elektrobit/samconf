# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
  test_samconf_samconfConfigMergeConfig_utest
  SOURCES
    case_success.c
    case_overwrite_success.c
    samconfConfigMergeConfig_utest.c
  LIBRARIES
    samconf_static
    samconf_test_utils
)
