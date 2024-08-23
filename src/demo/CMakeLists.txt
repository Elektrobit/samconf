# SPDX-License-Identifier: MIT
add_executable(
  samprobe
  samprobe.c
)

add_executable(
  samenvprobe
  samenvprobe.c
)

target_link_libraries(
  samprobe
  PRIVATE
    samconf
)

target_link_libraries(
  samenvprobe
  PRIVATE
    samconf
)
if (INSTALL_SAMCONF_DEMOS)
  install(TARGETS samprobe DESTINATION ${CMAKE_INSTALL_BINDIR})
  install(TARGETS samenvprobe DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (INSTALL_SAMCONF_DEMOS)
