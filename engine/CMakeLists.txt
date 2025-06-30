set(ENGINE_ROOT_DIR "${ROOT_DIR}/engine")
set(DEPENDENCY_DIR "${ENGINE_ROOT_DIR}/dependencies")

option(ENABLE_PHYSICS_DEBUG_RENDERER "Enable Physics Debug Renderer" OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options("/MP")
  set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT
                                                        ArcadiaEditor)
endif()

add_subdirectory(dependencies)

add_subdirectory(source/runtime)
add_subdirectory(source/editor)


