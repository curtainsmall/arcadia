set(glew_SOURCE_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/glew")

file(GLOB glew_sources CONFIURE_DEPENDS "${glew_SOURCE_DIR_}/*.h")

add_library(glew INTERFACE "${glew_sources}")
target_include_directories(
    glew INTERFACE $<BUILD_INTERFACE:${glew_SOURCE_DIR_}/include>)
