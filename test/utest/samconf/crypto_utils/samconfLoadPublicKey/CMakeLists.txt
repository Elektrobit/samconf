# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_samconfLoadPublicKey_utest
  SOURCES
    case_success.c
    case_nullpatherror.c
    case_fopenerror.c
    case_pemreaderror.c
    samconfLoadPublicKey_utest.c
  LIBRARIES
    samconf_weak
    mock_samconf_static
    safu::mock_safu
    cmocka_mocks::mock_libc
    cmocka_mocks::mock_openssl
)
