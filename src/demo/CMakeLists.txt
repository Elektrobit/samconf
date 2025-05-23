# SPDX-License-Identifier: MIT
add_compile_definitions(SAFU_LOG=1)
find_package(json-c REQUIRED) # the installed version of json-c isn't known to cmake

add_executable(
  samprobe
  samprobe.c
)

target_link_libraries(
  samprobe
  PRIVATE
    samconf
)

if (INSTALL_SAMCONF_DEMOS)
  install(TARGETS samprobe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (INSTALL_SAMCONF_DEMOS)

add_executable(
  env_example
  env_example.c
)

target_link_libraries(
  env_example
  PRIVATE
    samconf
)

add_executable(
  dump_config
  dump_config.c
)

target_link_libraries(
  dump_config
  PRIVATE
    samconf
)

add_executable(
  dump_merged_config
  dump_merged_config.c
)

target_link_libraries(
  dump_merged_config
  PRIVATE
    samconf
)

if (INSTALL_SAMCONF_DEMOS)
  install(TARGETS env_example DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS dump_config DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS dump_merged_config DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (INSTALL_SAMCONF_DEMOS)
