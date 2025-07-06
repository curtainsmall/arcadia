file(GLOB_RECURSE editor_header_files CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE editor_source_files CONFIGURE_DEPENDS "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${editor_header_files}
                                                      ${editor_source_files})

add_executable(${EDITOR_NAME} "${editor_header_files}"
                                  "${editor_source_files}")

set_target_properties(
  ${EDITOR_NAME}
  PROPERTIES # Set output name
  OUTPUT_NAME "ArcadiaEngine"
             # Set folder in solution view
             FOLDER "engine"
             # Use WinMain in release
             WIN32_EXECUTABLE $<CONFIG:Release>
             # Set debugger working directory to target file directory
             VS_DEBUGGER_WORKING_DIRECTORY
             "$<TARGET_FILE_DIR:${EDITOR_NAME}>")

# target_compile_features(${EDITOR_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${EDITOR_NAME} PRIVATE ${RUNTIME_NAME})

target_include_directories(${EDITOR_NAME} PUBLIC "${EDITOR_DIR}")

target_compile_features(${EDITOR_NAME} PUBLIC cxx_std_20)

set(POST_BUILD_COMMANDS
    # Copy shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/shaders"
    "$<TARGET_FILE_DIR:${EDITOR_NAME}>/shaders"
    # Copy fonds
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/fonts"
    "$<TARGET_FILE_DIR:${EDITOR_NAME}>/fonts"
    # Copy dlls
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EDITOR_NAME}>
    $<TARGET_RUNTIME_DLLS:${EDITOR_NAME}>
    $<TARGET_FILE_DIR:${EDITOR_NAME}> COMMAND_EXPAND_LISTS)

add_custom_command(TARGET ${EDITOR_NAME} ${POST_BUILD_COMMANDS})
