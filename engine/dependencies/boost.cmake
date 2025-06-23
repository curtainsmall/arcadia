set(boost_SOURCE_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/boost")

file(GLOB boost_sources CONFIURE_DEPENDS "${boost_SOURCE_DIR_}/*.h"
     "${boost_SOURCE_DIR_}/*.hpp")
add_library(boost INTERFACE "${boost_sources}")
target_include_directories(boost
                           INTERFACE $<BUILD_INTERFACE:${boost_SOURCE_DIR_}>)
