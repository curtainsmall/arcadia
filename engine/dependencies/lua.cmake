set(lua_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua")

file(GLOB lua_header_files CONFIGURE_DEPENEDS
     "${lua_SOURCE_DIR}/*.h")
file(GLOB lua_source_files CONFIGURE_DEPENEDS
    "${lua_SOURCE_DIR}/*.c")

list(REMOVE_ITEM lua_source_files "${lua_SOURCE_DIR}/onelua.c")

add_library(lua SHARED ${lua_header_files} ${lua_source_files})

target_include_directories(lua PUBLIC "${lua_SOURCE_DIR}")

target_compile_definitions(lua PRIVATE LUA_BUILD_AS_DLL)

target_compile_features(lua PRIVATE c_std_99)

 


