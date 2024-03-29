# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.21)

include(cmake/project.cmake)

project(
  samconf
    DESCRIPTION "Signed And Mergeable CONFiguration"
    VERSION ${SAMCONF_VERSION}
    LANGUAGES C
)

project_set_environment()
project_set_version_variables()
project_add_documentation_target(
  TITLE
    "BaseOS SamConf Documentation"
  MARKDOWN
    ${CMAKE_SOURCE_DIR}/documentation/documentation.md
)

option(JSON_BACKEND "Enable Json Backend Support" ON)

if(JSON_BACKEND)
	add_compile_options(-DSAMCONF_ENABLE_CONFIG_BACKEND_JSON)
endif(JSON_BACKEND)

add_subdirectory(src)
add_subdirectory(test/utest)

if(PACKAGING)
  include(cmake/Packing.cmake)
endif(PACKAGING)
