# SPDX-License-Identifier: MIT
find_package(cmocka_extensions 0.53.1 REQUIRED)

create_unit_test(
  NAME
    test_samconfGetBackendOps_utest
  SOURCES
    case_success_get_dummy_backend.c
    case_success_get_json_backend.c
    samconfGetBackendOps_utest.c
  LIBRARIES
    cmocka_extensions::cmocka_extensions
    samconf_weak
    mock_samconf_static
)
