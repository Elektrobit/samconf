# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_samconfBase64Decode_utest
  SOURCES
    case_success.c
    case_allocerror.c
    case_decodeerror.c
    case_paddingerror.c
    samconfBase64Decode_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_openssl
)
