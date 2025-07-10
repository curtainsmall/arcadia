set(bootstrap_source_files "main.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${bootstrap_source_files})

add_executable(${BOOTSTRAP_NAME} ${bootstrap_source_files})

set_target_properties(
  ${BOOTSTRAP_NAME}
  PROPERTIES OUTPUT_NAME "Arcadia"
             FOLDER "engine"
             WIN32_EXECUTABLE $<CONFIG:Release>
             VS_DEBUGGER_WORKING_DIRECTORY
             "$<TARGET_FILE_DIR:${BOOTSTRAP_NAME}>")

target_link_libraries(${BOOTSTRAP_NAME} PRIVATE ${EDITOR_NAME} ${RUNTIME_NAME})

target_include_directories(${BOOTSTRAP_NAME} PRIVATE ${BOOTSTRAP_DIR})

target_compile_features(${BOOTSTRAP_NAME} PRIVATE cxx_std_20)

set(POST_BUILD_COMMANDS
    # Copy shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/shaders"
    "$<TARGET_FILE_DIR:${BOOTSTRAP_NAME}>/shaders"
    # Copy fonds
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/fonts"
    "$<TARGET_FILE_DIR:${BOOTSTRAP_NAME}>/fonts"
    # Copy dlls
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${BOOTSTRAP_NAME}>
    $<TARGET_RUNTIME_DLLS:${BOOTSTRAP_NAME}>
    $<TARGET_FILE_DIR:${BOOTSTRAP_NAME}> COMMAND_EXPAND_LISTS)

add_custom_command(TARGET ${BOOTSTRAP_NAME} ${POST_BUILD_COMMANDS})
