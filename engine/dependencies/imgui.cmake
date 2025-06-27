set(imgui_SOURCE_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

file(GLOB imgui_sources CONFIURE_DEPENDS "${imgui_SOURCE_DIR_}/*.cpp"
     "${imgui_SOURCE_DIR_}/*.h")
file(GLOB imgui_misc_sources CONFIURE_DEPENDS
     "${imgui_SOURCE_DIR_}/misc/cpp/imgui_stdlib.h"
     "${imgui_SOURCE_DIR_}/misc/cpp/imgui_stdlib.cpp")
file(
  GLOB
  imgui_impl_sources
  CONFIURE_DEPENDS
  "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
  "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.cpp"
  "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.h"
  "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3_loader.h")
add_library(imgui STATIC "${imgui_sources}" "${imgui_impl_sources}"
                         "${imgui_misc_sources}")
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>)
find_package(OpenGL)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
