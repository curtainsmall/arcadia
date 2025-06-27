set(gl3w_SOURCE_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/gl3w")

file(GLOB gl3w_sources CONFIURE_DEPENDS "${gl3w_SOURCE_DIR_}/src/gl3w.c"
    "${gl3w_SOURCE_DIR_}/include/GL/*.h" "${gl3w_SOURCE_DIR_}/include/KHR/*.h")
 add_library(gl3w STATIC "${gl3w_sources}")
 target_include_directories(gl3w PUBLIC $<BUILD_INTERFACE:${gl3w_SOURCE_DIR_}/include>)
 find_package(OpenGL)
 target_link_libraries(gl3w PUBLIC OpenGL::GL)
