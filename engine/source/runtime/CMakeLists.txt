# Runtime lib
file(GLOB_RECURSE runtime_header_files CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE runtime_source_files CONFIGURE_DEPENDS "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${runtime_header_files}
                                                      ${runtime_source_files})
add_library(${RUNTIME_NAME} SHARED ${runtime_source_files}
                                   ${runtime_header_files})

add_compile_definitions(ACDA_RUNTIME_EXPORT ACDA_RUNTIME)

set_target_properties(${RUNTIME_NAME} PROPERTIES FOLDER "engine")

# Enforce standards conformance on MSVC
target_compile_options(${RUNTIME_NAME}
                       PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
# target_compile_options(${RUNTIME_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC:/WX->")

target_link_libraries(
  ${RUNTIME_NAME}
  PUBLIC assimp
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
    "${DEPENDENCY_DIR}"
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
  ${RUNTIME_NAME}
  # PUBLIC $<BUILD_INTERFACE:${RUNTIME_DEPENDENCY_INCLUDES}>
  PUBLIC "${RUNTIME_DIR}"
  PUBLIC "${DEPENDENCY_DIR}")

target_precompile_headers(
  ${RUNTIME_NAME}
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

target_compile_features(${RUNTIME_NAME} PUBLIC cxx_std_20)
