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
option(ENV_BACKEND "Enable Environment Variable Backend Support" ON)
option(ENABLE_LOGGING "Enable the safuLog fucntions" OFF)

if(JSON_BACKEND)
	add_compile_options(-DSAMCONF_ENABLE_CONFIG_BACKEND_JSON)
endif(JSON_BACKEND)

if(ENV_BACKEND)
	add_compile_options(-DSAMCONF_ENABLE_CONFIG_BACKEND_ENV)
endif(ENV_BACKEND)

if(ENABLE_LOGGING)
	add_compile_definitions(SAFU_LOG=1)
endif(ENABLE_LOGGING)

add_subdirectory(src)
add_subdirectory(test/utest)

if(PACKAGING)
  include(cmake/Packing.cmake)
endif(PACKAGING)
