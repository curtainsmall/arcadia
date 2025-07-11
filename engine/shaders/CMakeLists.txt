file(GLOB_RECURSE shader_files CONFIGURE_DEPENDS "*.vert" "*.frag")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${shader_files})

add_custom_target(${SHADERS_NAME} SOURCES ${shader_files})

set_target_properties(${SHADERS_NAME} PROPERTIES FOLDER "engine/shaders")
