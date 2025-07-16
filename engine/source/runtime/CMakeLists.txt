# Runtime lib
file(GLOB_RECURSE runtime_header_files CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE runtime_source_files CONFIGURE_DEPENDS "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${runtime_header_files}
                                                      ${runtime_source_files})
add_library(${RUNTIME_NAME} SHARED ${runtime_source_files}
                                   ${runtime_header_files})

target_compile_definitions(${RUNTIME_NAME} PRIVATE ACDA_RUNTIME_EXPORT
                                                   ACDA_RUNTIME)

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
         imgui_console
         Jolt
         EnTT
         nlohmann_json
         portable_file_dialogs
         lua)

target_include_directories(
  ${RUNTIME_NAME}
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
