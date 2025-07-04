# SPDX-License-Identifier: MIT
set(SAMCONF_VERSION 0.75.4)

# Attention: Aside from the version, as many things as possible in this file
#   should be put into functions, as this solves potential issues with commands
#   being run before "project()" gets called in the including CMakeLists.txt.
#   It also does not force the usage of everything at once which can be useful,
#   should we need to do things different in one of the projects in the future.

macro(project_set_environment)
  set(CMAKE_C_STANDARD 17)
  set(CMAKE_C_STANDARD_REQUIRED ON)
  set(CMAKE_C_EXTENSIONS OFF)
  set(CMAKE_C_FLAGS_DEBUG "")
  set(CMAKE_C_FLAGS_RELEASE "")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  include(GNUInstallDirs)

  option(SAMCONF_BUILD_DEFAULTS "enable all default builds" ON)

  option(ENABLE_ANALYZER "Build with -fanalyzer" ON)
  option(ENABLE_CI "Use CI mode for building" OFF)
  option(ENABLE_ASAN "Link with ASAN" ON)
  option(ENABLE_GIT_VERSION "Enable the git hash for the version" ON)

  option(UNIT_TESTS "Build unit tests" ${SAMCONF_BUILD_DEFAULTS})
  option(INSTALL_UNIT_TESTS "Install unit tests" ${UNIT_TESTS})

  option(SAMCONF_TEST_UTILS "Build utility library for easy unit tests" ${UNIT_TESTS})
  option(INSTALL_SAMCONF_TEST_UTILS "Install unit test utility library" ${SAMCONF_TEST_UTILS})

  option(SAMCONF_MOCK_LIBRARY "Build the mock library" ${SAMCONF_BUILD_DEFAULTS})
  option(INSTALL_SAMCONF_MOCK_LIBRARY "Install the mock library" ${SAMCONF_MOCK_LIBRARY})

  option(SAMCONF_TOOLS "Build samconf tools like signature.sh" ${SAMCONF_BUILD_DEFAULTS})
  option(INSTALL_SAMCONF_TOOLS "Install the samconf tools" ${SAMCONF_TOOLS})

  option(SAMCONF_DEMOS "Build the samconf demos" ${SAMCONF_BUILD_DEFAULTS})
  option(INSTALL_SAMCONF_DEMOS "Install the samconf demos" ${SAMCONF_DEMOS})

  add_compile_options(
    -Wshadow -Wall -Wextra -pedantic -D_DEFAULT_SOURCE
  )

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (ENABLE_ASAN)
      link_libraries(asan)
      add_compile_options(-fsanitize=address)
    endif()
    add_compile_options(
      -Og -g3 -DDEBUG -fno-omit-frame-pointer
      $<IF:$<BOOL:${ENABLE_ANALYZER}>,-fanalyzer,>
      $<IF:$<BOOL:${ENABLE_ANALYZER}>,-Wno-analyzer-malloc-leak,>
    )
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG -g $<IF:$<BOOL:${ENABLE_CI}>,-Werror,>)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Profiling")
    add_compile_options(-O3 -DNDEBUG -g -fno-omit-frame-pointer)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Tsan")
    link_libraries(tsan)
    add_compile_options(
      -Og -g3 -DDEBUG -fsanitize=thread -fno-omit-frame-pointer
      $<IF:$<BOOL:${ENABLE_ANALYZER}>,-fanalyzer,>
      $<IF:$<BOOL:${ENABLE_ANALYZER}>,-Wno-analyzer-malloc-leak,>
    )
  endif()
endmacro()

function(project_set_version_variables)
  if(ARGN)
    cmake_parse_arguments(PSVV "" "NAME" "" ${ARGN})
    set(prj_name ${PSVV_NAME})
  else()
    set(prj_name ${PROJECT_NAME})
  endif()
  string(TOUPPER ${prj_name} prefix)
  set(ver_major_name "${prefix}_VERSION_MAJOR")
  set(ver_minor_name "${prefix}_VERSION_MINOR")
  set(ver_micro_name "${prefix}_VERSION_MICRO")
  set(ver_git_name "${prefix}_VERSION_GIT")

  set(ver_pkg "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
  set(${ver_major_name} ${PROJECT_VERSION_MAJOR} PARENT_SCOPE)
  set(${ver_minor_name} ${PROJECT_VERSION_MINOR} PARENT_SCOPE)
  set(${ver_micro_name} ${PROJECT_VERSION_PATCH} PARENT_SCOPE)
  set(PKG_VERSION ${ver_pkg} PARENT_SCOPE)

  if (ENABLE_GIT_VERSION)
    find_package(Git REQUIRED)
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE exit_code
      OUTPUT_VARIABLE git_output
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT exit_code EQUAL 0)
      message(WARNING "failed to retrive git short commit hash")
      set(git_output "none")
    endif()
  else ()
    set(git_output "none")
  endif ()
  set(${ver_git_name} ${git_output} PARENT_SCOPE)
  message(STATUS "-- Set version for ${prj_name}: ${ver_pkg}.${git_output}")
endfunction()


function(project_add_documentation_target)
  if (TARGET ${PROJECT_NAME}_doc)
    return()
  endif ()
  cmake_parse_arguments(PARAM "" "MARKDOWN;TITLE" "" ${ARGN})

  find_program(PLANTUML NAMES plantuml)
  find_program(PANDOC NAMES pandoc)
  add_custom_target(
    ${PROJECT_NAME}_doc
    mkdir -p ${CMAKE_BINARY_DIR}/doc/images &&
    ${PLANTUML} ${CMAKE_SOURCE_DIR}/documentation/images/*.plantuml &&
    ${PANDOC} -s --from gfm --to html --filter pandoc-plantuml -o ${CMAKE_BINARY_DIR}/doc/${PROJECT_NAME}_doc.html --css style.css ${PARAM_MARKDOWN} --metadata title='${PARAM_TITLE}' &&
    cmake -E copy_if_different ${CMAKE_SOURCE_DIR}/documentation/style.css doc/ &&
    cmake -E copy_if_different ${CMAKE_SOURCE_DIR}/documentation/images/*.png doc/images/
  )
endfunction()
