cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

project(arcadia VERSION 0.1.0)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source build is NOT allowed.")
endif()

#Config
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to Release since there it is not specified.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX "${ROOT_DIR}/bin")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}")

add_subdirectory(engine)
