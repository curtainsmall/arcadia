cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

<<<<<<< Updated upstream
project(arcadia)
||||||| Stash base
project(Arcadia)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source build is NOT allowed.")
endif()
=======
project(Arcadia VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source build is NOT allowed.")
endif()
>>>>>>> Stashed changes

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to Release since there it is not specified.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

<<<<<<< Updated upstream
set(3RDPARTY_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/")
set(EDITOR_APP_NAME ${PROJECT_NAME}_editor)
set(RUNTIME_LIB_NAME ${PROJECT_NAME}_runtime)
||||||| Stash base
set(DEPENDENCY_LIB_DIR "${PROJECT_SOURCE_DIR}/dependencies")
=======
set(ARCADIA_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX "${ARCADIA_ROOT_DIR}/bin")
set(ARCADIA_BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}")
>>>>>>> Stashed changes

<<<<<<< Updated upstream
# 3rdparty Libraries
||||||| Stash base
add_subdirectory()
=======
add_subdirectory(engine)
>>>>>>> Stashed changes

# Runtime Library
add_library(RUNTIME_LIB_NAME DYNAMIC)
set_target_properties(RUNTIME_LIB_NAME PROPERTY CXX_STANDARD 20)
target_include_directories(
  RUNTIME_LIB_NAME
  PRIVATE "runtime/src/"
  PRIVATE "${3RDPARTY_LIB_DIR}"
  PRIVATE "${3RDPARTY_LIB_DIR}assimp/include"
  PRIVATE "${3RDPARTY_LIB_DIR}gl3w/include"
  PRIVATE "${3RDPARTY_LIB_DIR}JoltPhysics"
  PRIVATE "${3RDPARTY_LIB_DIR}glm"
  PRIVATE "${3RDPARTY_LIB_DIR}imguizimo"
  PRIVATE "${3RDPARTY_LIB_DIR}imgui"
  PRIVATE "${3RDPARTY_LIB_DIR}glfw/inlcude"
  PRIVATE "${3RDPARTY_LIB_DIR}spdlog/include"
  PRIVATE "${3RDPARTY_LIB_DIR}portable-file-dialogs"
  PRIVATE "${3RDPARTY_LIB_DIR}boost_1_83_0"
  PRIVATE "${3RDPARTY_LIB_DIR}glew/include")

# Editor Executable
add_executable(EDITOR_APP_NAME)
set_target_properties(EDITOR_APP_NAME PROPERTY CXX_STANDARD 20)
target_link_libraries(EDITOR_APP_NAME RUNTIME_LIB_NAME)

# Bootstrap for the final game TODO
