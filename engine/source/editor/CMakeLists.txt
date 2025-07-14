file(GLOB_RECURSE editor_header_files CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE editor_source_files CONFIGURE_DEPENDS "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${editor_header_files}
                                                      ${editor_source_files})

add_library(${EDITOR_NAME} SHARED "${editor_header_files}"
                                  "${editor_source_files}")

target_compile_definitions(${EDITOR_NAME} PRIVATE ACDA_EDITOR_EXPORT
                                                  ACDA_EDITOR)

set_target_properties(${EDITOR_NAME} PROPERTIES FOLDER "engine")

# Enforce standards conformance on MSVC
target_compile_options(${RUNTIME_NAME}
                       PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# target_compile_features(${EDITOR_NAME} PUBLIC
# "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${EDITOR_NAME} PRIVATE ${RUNTIME_NAME})

target_include_directories(${EDITOR_NAME} PUBLIC "${EDITOR_DIR}")

target_compile_features(${EDITOR_NAME} PUBLIC cxx_std_20)
