set(ENGINE_ROOT_DIR "${ROOT_DIR}/engine")
set(DEPENDENCY_DIR "${ENGINE_ROOT_DIR}/dependencies")
set(RUNTIME_NAME arcadia_runtime)
set(RUNTIME_DIR "${ENGINE_ROOT_DIR}/source/runtime")
set(EDITOR_NAME arcadia_editor)
set(EDITOR_DIR "${ENGINE_ROOT_DIR}/source/editor")
set(BOOTSTRAP_NAME arcadia_bootstrap)
set(BOOTSTRAP_DIR "${ENGINE_ROOT_DIR}/source/bootstrap")
set(TEST_NAME arcadia_test)
set(TEST_DIR "${ENGINE_ROOT_DIR}/test")

option(ENABLE_PHYSICS_DEBUG_RENDERER "Enable Physics Debug Renderer" OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options("/MP")
  set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT
      ${BOOTSTRAP_NAME})
endif()

#MSVC
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_subdirectory(dependencies)

add_subdirectory(source/runtime)
add_subdirectory(source/editor)
add_subdirectory(source/bootstrap)

enable_testing()
add_subdirectory(test)
