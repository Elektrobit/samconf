# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)
find_package(cmocka_extensions 0.53.1 REQUIRED)

create_unit_test(
  NAME
    test_samconfInsertAt_utest
  SOURCES
    case_success.c
    samconfInsertAt_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
