# SPDX-License-Identifier: MIT
add_compile_definitions(SAFU_LOG=1)
add_executable(
  samprobe
  samprobe.c
)

add_executable(
  env_example
  env_example.c
)

target_link_libraries(
  samprobe
  PRIVATE
    samconf
)

target_link_libraries(
  env_example
  PRIVATE
    samconf
)
if (INSTALL_SAMCONF_DEMOS)
  install(TARGETS samprobe DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS env_example DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (INSTALL_SAMCONF_DEMOS)
