# SPDX-License-Identifier: MIT
find_package(safu 0.56.2 REQUIRED)
find_package(cmocka_mocks 0.54.2 REQUIRED)

create_unit_test(
  NAME
    test_samconfSignatureLoaderGetSignatureFor_utest
  SOURCES
    case_success.c
    case_allocerror.c
    case_accesserror.c
    case_readerror.c
    samconfSignatureLoaderGetSignatureFor_utest.c
  LIBRARIES
    samconf_weak
    safu::mock_safu
    cmocka_mocks::mock_libc
)
