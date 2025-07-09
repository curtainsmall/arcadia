set(GTEST_DIR "${TEST_DIR}/googletest")
set(TEST_SOURCE_DIR "${TEST_DIR}/source")
set(TEST_NAME arcadia_test)

set(test_folder "test")

file(GLOB_RECURSE test_header_files CONFIGURE_DEPENDS
     "${TEST_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE test_source_files CONFIGURE_DEPENDS
     "${TEST_SOURCE_DIR}/*.cpp")

if(NOT TARGET gtest)
  # For Windows: Prevent overriding the parent project's compiler/linker
  # settings
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE)
  option(INSTALL_GTEST "" OFF)
  add_subdirectory(googletest)
  set_target_properties(gtest PROPERTIES FOLDER "${test_folder}")
endif()

source_group(TREE "${TEST_SOURCE_DIR}" FILES ${test_header_files}
                                             ${test_source_files})

add_executable(${TEST_NAME} ${test_header_files} ${test_source_files})
set_target_properties(
  ${TEST_NAME}
  PROPERTIES OUTPUT_NAME "ArcadiaEngineTest"
             FOLDER "${test_folder}"
             VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TEST_NAME}>")
target_include_directories(${TEST_NAME} PRIVATE "${TEST_SOURCE_DIR}")
target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main)
target_compile_features(${TEST_NAME} PUBLIC cxx_std_20)

set(POST_BUILD_COMMANDS
    # Copy dlls
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_NAME}>
    $<TARGET_RUNTIME_DLLS:${TEST_NAME}>
    $<TARGET_FILE_DIR:${TEST_NAME}> COMMAND_EXPAND_LISTS)
add_custom_command(TARGET ${TEST_NAME} ${POST_BUILD_COMMANDS})

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
