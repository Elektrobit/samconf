# SPDX-License-Identifier: MIT
find_package(json-c REQUIRED)
find_package(safu REQUIRED)

add_library(
  samconf_test_utils
  SHARED
    private/createMockConfigUtil.c
)

target_include_directories(
  samconf_test_utils
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
  $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
  samconf_test_utils
  PUBLIC
    samconf
)

set_target_properties(
  samconf_test_utils
  PROPERTIES
    SOVERSION ${SAMCONF_VERSION_MAJOR}
    VERSION ${SAMCONF_VERSION_MAJOR}.${SAMCONF_VERSION_MINOR}.${SAMCONF_VERSION_MICRO}
)

if (SAMCONF_TEST_UTILS AND INSTALL_SAMCONF_TEST_UTILS)
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    "Config.cmake.in"
    "samconf/samconf_test_utilsConfig.cmake"
    INSTALL_DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/cmake/samconf
    PATH_VARS
      CMAKE_INSTALL_LIBDIR
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/samconf/samconf_test_utilsConfigVersion.cmake
    VERSION ${PKG_VERSION}
      COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/samconf/samconf_test_utilsConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/samconf/samconf_test_utilsConfigVersion.cmake"
    DESTINATION
      "${CMAKE_INSTALL_LIBDIR}/cmake/samconf"
  )

  install(TARGETS samconf_test_utils EXPORT samconfTargets  DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(DIRECTORY public/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
endif (SAMCONF_TEST_UTILS AND INSTALL_SAMCONF_TEST_UTILS)
