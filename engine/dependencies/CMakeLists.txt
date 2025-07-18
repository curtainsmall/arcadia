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

# if(NOT TARGET glew) add_subdirectory(glew/build/cmake)
# set_target_properties(glew PROPERTIES FOLDER "${dependency_folder}/glew")
# endif()

if(NOT TARGET gl3w)
  include(gl3w.cmake)
  set_target_properties(gl3w PROPERTIES FOLDER "${dependency_folder}/gl3w")
endif()

if(NOT TARGET boost)
  include(boost.cmake)
  set_target_properties(boost PROPERTIES FOLDER "${dependency_folder}/boost")
endif()

if(NOT TARGET EnTT)
  add_subdirectory(entt)
  set_target_properties(EnTT PROPERTIES FOLDER "${dependency_folder}/entt")
endif()

if(NOT TARGET portable_file_dialogs)
  add_subdirectory(portable-file-dialogs)
  set_target_properties(
    portable_file_dialogs
    PROPERTIES FOLDER "${dependency_folder}/portable-file-dialogs")
endif()

if(NOT TARGET spdlog)
  option(SPDLOG_BUILD_EXAMPLES "" OFF)
  option(SPDLOG_INSTALL "" OFF)
  add_subdirectory(spdlog)
  set_target_properties(spdlog PROPERTIES FOLDER "${dependency_folder}/spdlog")
endif()

if(NOT TARGET glm)
  option(GLM_ENABLE_CXX_20 "" ON)
  option(GLM_BUILD_INSTALL "" OFF)
  option(GLM_BUILD_LIBRARY "" OFF)
  option(GLM_BUILD_TESTS "" OFF)
  add_subdirectory(glm)
  set_target_properties(glm PROPERTIES FOLDER "${dependency_folder}/glm")
endif()

if(NOT TARGET nlohmann_json)
  set(JSON_BuildTests
      OFF
      CACHE INTERNAL "")
  add_subdirectory(nlohmann_json)
endif()

if(NOT TARGET stb)
  include(stb.cmake)
  set_target_properties(stb PROPERTIES FOLDER "${dependency_folder}/stb")
endif()

if(NOT TARGET assimp)
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
  option(ENABLE_INSTALL "" OFF)
  option(JPH_SHARED_LIBRARY "" ON)
  option(JPH_DEBUG_RENDERER "" OFF)
  option(USE_STATIC_MSVC_RUNTIME_LIBRARY "" OFF)

  if(ENABLE_PHYSICS_DEBUG_RENDERER)
    option(TARGET_TEST_FRAMEWORK "" ON)
  else()
    option(TARGET_TEST_FRAMEWORK "" OFF)
  endif()

  add_subdirectory(jolt_physics/Build)

  set_target_properties(Jolt PROPERTIES FOLDER "${dependency_folder}/jolt")

  # if(ENABLE_PHYSICS_DEBUG_RENDERER) set_target_properties( Jolt TestFramework
  # PROPERTIES FOLDER "${dependency_folder}/jolt_physics" MSVC_RUNTIME_LIBRARY
  # "MultiThreadedDLL") else() set_target_properties( Jolt PROPERTIES FOLDER
  # "${dependency_folder}/jolt_physics" MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  # endif()

endif()

if(NOT TARGET lua)
  include(lua.cmake)
  set_target_properties(lua PROPERTIES FOLDER "${dependency_folder}/lua")
endif()

if(NOT TARGET csys)
  add_subdirectory(csys)
  set_target_properties(csys PROPERTIES FOLDER
                                                    "${dependency_folder}/csys")
endif()
