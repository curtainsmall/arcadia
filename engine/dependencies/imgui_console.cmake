set(imgui_console_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui-console")

file(GLOB imgui_console_header_files CONFIURE_DEPENDS
     "${imgui_console_SOURCE_DIR}/include/imgui_console/*.h")
file(GLOB imgui_console_source_files CONFIURE_DEPENDS
     "${imgui_console_SOURCE_DIR}/src/*.cpp")

add_library(imgui_console STATIC ${imgui_console_header_files}
                                 ${imgui_console_source_files})

target_include_directories(
  imgui_console PUBLIC "${imgui_console_SOURCE_DIR}/include/"
                       "${imgui_console_SOURCE_DIR}/include/imgui_console/")
target_link_libraries(imgui_console PUBLIC imgui)

target_compile_features(imgui_console PUBLIC cxx_std_20)
