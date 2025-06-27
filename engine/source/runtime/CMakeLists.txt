set(RUNTIME_LIB_NAME ArcadiaRuntime)
set(RUNTIME_LIB_SOURCE_DIR "${ENGINE_ROOT_DIR}/source/runtime")

# Runtime lib
file(GLOB_RECURSE RUNTIME_LIB_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE RUNTIME_LIB_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES "${RUNTIME_LIB_HEADERS}"
                                                      "${RUNTIME_LIB_SOURCES}")
add_library(${RUNTIME_LIB_NAME} ${RUNTIME_LIB_SOURCES} ${RUNTIME_LIB_HEADERS})

add_compile_definitions(ACDA_DYNAMIC_RUNTIME_LIB ACDA_EXPORT)

set_target_properties(${RUNTIME_LIB_NAME} PROPERTIES CXX_STANARD 20)
set_target_properties(${RUNTIME_LIB_NAME} PROPERTIES FOLDER "engine")

# Enforce standards conformance on MSVC
target_compile_options(${RUNTIME_LIB_NAME}
                       PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
# target_compile_options(${RUNTIME_LIB_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC:/WX->")

target_link_libraries(
  ${RUNTIME_LIB_NAME}
  PUBLIC
  assimp
  boost
  spdlog::spdlog
  stb
  glfw
  gl3w
  glm::glm-header-only
  boost
  imgui
  imguizmo
  Jolt
  EnTT
  nlohmann_json
  portable_file_dialogs)

set(RUNTIME_DEPENDENCY_INCLUDES
    "${ENGINE_ROOT_DIR}/source/runtime"
    # "${DEPENDENCY_DIR}"
    "${DEPENDENCY_DIR}/assimp/include"
    "${DEPENDENCY_DIR}/boost"
    "${DEPENDENCY_DIR}/entt/src"
    # "${DEPENDENCY_DIR}/glew/include"
    "${DEPENDENCY_DIR}/glfw/include"
    "${DEPENDENCY_DIR}/glm/"
    "${DEPENDENCY_DIR}/imgui"
    "${DEPENDENCY_DIR}/imguizmo"
    "${DEPENDENCY_DIR}/jolt_physics"
    "${DEPENDENCY_DIR}/nlohmann_json/include"
    "${DEPENDENCY_DIR}/portable-file-dialogs"
    "${DEPENDENCY_DIR}/spdlog/include"
    "${DEPENDENCY_DIR}/stb")

target_include_directories(
  ${RUNTIME_LIB_NAME}
  # PUBLIC $<BUILD_INTERFACE:${RUNTIME_DEPENDENCY_INCLUDES}>
  PUBLIC "${RUNTIME_LIB_SOURCE_DIR}"
  PUBLIC "${DEPENDENCY_DIR}")

target_precompile_headers(
  ${RUNTIME_LIB_NAME}
  PUBLIC
  <algorithm>
  <array>
  <cassert>
  <chrono>
  <deque>
  <filesystem>
  <functional>
  <memory>
  <optional>
  <queue>
  <stack>
  <string>
  <unordered_map>
  <utility>
  <vector>
  platform/opengl.hpp
  platform/glfw.hpp
  platform/jolt.hpp
  resource/entt.hpp)

target_compile_features(${RUNTIME_LIB_NAME} PUBLIC cxx_std_20)
