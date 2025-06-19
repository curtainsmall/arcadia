set(RUNTIME_LIB_NAME ArcadiaRuntime)
set(RUNTIME_LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/runtime/src")


# Runtime lib
file(GLOB_RECURSE RUNTIME_LIB_HEADERS "*.hpp")
file(GLOB_RECURSE RUNTIME_LIB_SOURCES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES "${RUNTIME_LIB_HEADERS}"
                                                      "${RUNTIME_LIB_SOURCES}")
add_library(${RUNTIME_LIB_NAME} ${RUNTIME_LIB_SOURCES} ${RUNTIME_LIB_HEADERS})

set_target_properties(${RUNTIME_LIB_NAME} PROPERTIES CXX_STANARD 20)
set_target_properties(${RUNTIME_LIB_NAME} PROPERTIES FOLDER "runtime")

# Enforce standards conformance on MSVC
target_compile_options(${RUNTIME_LIB_NAME}
                       PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
# target_compile_options(${RUNTIME_LIB_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC:/WX->")

target_link_libraries(
  ${RUNTIME_LIB_NAME}
  PUBLIC spdlog::spdlog
  PRIVATE Assimp stb
  PUBLIC glfw
  PUBLIC imgui
  PUBLIC imguizmo
  PUBLIC Jolt
  PUBLIC EnTT
  PUBLIC portable-file-dialogs)

file(GLOB RUNTIME_DEPENDENCY_INCLUDES 
    "${DEPENDENCY_DIR}/**/include")

target_include_directories(
    ${RUNTIME_LIB_NAME}
    PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source>
    PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source/runtime>
    PUBLIC $<BUILD_INTERFACE:${RUNTIME_DEPENDENCY_INCLUDES}>
)

target_compile_features(${RUNTIME_LIB_NAME} PUBLIC cxx_std_20)


