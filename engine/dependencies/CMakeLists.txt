set(dependency_folder "dependencies")

set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL "" FORCE)

if(NOT TARGET imgui)
  include(imgui.cmake)
  set_target_properties(imgui PROPERTIES FOLDER "${dependency_folder}/imgui")
endif()

if(NOT TARGET imguizmo)
  include(imguizmo.cmake)
  set_target_properties(imguizmo PROPERTIES FOLDER
                                            "${dependency_folder}/imguizmo")
endif()

if(NOT TARGET glfw)
  option(GLFW_BUILD_EXAMPLES "" OFF)
  option(GLFW_BUILD_TESTS "" OFF)
  option(GLFW_BUILD_DOCS "" OFF)
  option(GLFW_INSTALL "" OFF)
  add_subdirectory(glfw)
  set_target_properties(glfw PROPERTIES FOLDER "${dependency_folder}/glfw")
  set_target_properties(update_mappings PROPERTIES FOLDER
                                                   "${depndency_folder}/glfw")
endif()

if(NOT TARGET EnTT)
  add_subdirectory(entt)
endif()

if(NOT TARGET portable-file-dialogs)
  add_subdirectory(portable-file-dialogs)
endif()

if(NOT TARGET spdlog)
  option(SPDLOG_BUILD_EXAMPLES "" OFF)
  option(SPDLOG_INSTALL "" OFF)
  add_subdirectory(spdlog)
  set_target_properties(spdlog PROPERTIES FOLDER "${dependency_folder}/spdlog")
endif()

if(NOT TARGET stb)
  include(stb.cmake)
  set_target_properties(stb PROPERTIES FOLDER "${dependency_folder}/stb")
endif()

if(NOT TARGET Assimp)
  option(ASSIMP_BUILD_TESTS "" OFF)
  option(ASSIMP_WARNINGS_AS_ERRORS "" OFF)
  add_subdirectory(assimp)
  set_target_properties(assimp PROPERTIES FOLDER "${dependency_folder}/assimp")
endif()

if(NOT TARGET Jolt)
  option(TARGET_HELLO_WORLD "" OFF)
  option(TARGET_PERFORMANCE_TEST "" OFF)
  option(TARGET_SAMPLES "" OFF)
  option(TARGET_UNIT_TESTS "" OFF)
  option(TARGET_VIEWER "" OFF)

  if(ENABLE_PHYSICS_DEBUG_RENDERER)
    option(TARGET_TEST_FRAMEWORK "" ON)
  else()
    option(TARGET_TEST_FRAMEWORK "" OFF)
  endif()

  add_subdirectory(jolt_physics/Build)

  if(ENABLE_PHYSICS_DEBUG_RENDERER)
    set_target_properties(
      Jolt TestFramework PROPERTIES FOLDER "${dependency_folder}/jolt_physics"
                                    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  else()
    set_target_properties(
      Jolt PROPERTIES FOLDER "${dependency_folder}/jolt_physics"
                      MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  endif()

endif()
