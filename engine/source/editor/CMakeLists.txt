set(EDITOR_APP_NAME ArcadiaEditor)
set(EDITOR_APP_SOURCE_DIR "${ENGINE_ROOT_DIR}/source/editor")

file(GLOB_RECURSE EDITOR_APP_HEADERS CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE EDITOR_APP_SOURCES CONFIGURE_DEPENDS "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_APP_HEADERS}
                                                      ${EDITOR_APP_SOURCES})

add_executable(${EDITOR_APP_NAME} "${EDITOR_APP_HEADERS}"
                                  "${EDITOR_APP_SOURCES}")

set_target_properties(
  ${EDITOR_APP_NAME}
  PROPERTIES # Set output name
             OUTPUT_NAME "${EDITOR_APP_NAME}"
             # Set folder in solution view
             FOLDER "engine"
             # Use WinMain in release
             WIN32_EXECUTABLE $<CONFIG:Release>
             # Set debugger working directory to target file directory
             VS_DEBUGGER_WORKING_DIRECTORY
             "$<TARGET_FILE_DIR:${EDITOR_APP_NAME}>")

# target_compile_features(${EDITOR_APP_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${EDITOR_APP_NAME} PRIVATE ArcadiaRuntime)

target_include_directories(${EDITOR_APP_NAME} PUBLIC "${EDITOR_APP_SOURCE_DIR}")

set(POST_BUILD_COMMANDS
    # Copy shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/shaders"
    "$<TARGET_FILE_DIR:${EDITOR_APP_NAME}>/shaders"
    # Copy fonds
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/fonts"
    "$<TARGET_FILE_DIR:${EDITOR_APP_NAME}>/fonts"
    # Copy dlls
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EDITOR_APP_NAME}>
    $<TARGET_RUNTIME_DLLS:${EDITOR_APP_NAME}>
    $<TARGET_FILE_DIR:${EDITOR_APP_NAME}> COMMAND_EXPAND_LISTS)

add_custom_command(TARGET ${EDITOR_APP_NAME} ${POST_BUILD_COMMANDS})
