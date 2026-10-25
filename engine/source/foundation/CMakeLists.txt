# foundation ring
#
# Rules of this ring:
#   - no OS, graphics, physics or scripting library,
#   - only header-only helpers (nlohmann_json, glm) may be linked,
#   - everything else in the engine may depend on it, it depends on nothing
#     but those helpers. Keep it that way: it is what makes the rest testable.

add_library(arcadia_foundation STATIC
  event_bus.cpp)

target_include_directories(arcadia_foundation PUBLIC "${ENGINE_ROOT_DIR}/source")

target_compile_features(arcadia_foundation PUBLIC cxx_std_20)

target_link_libraries(
  arcadia_foundation
  PUBLIC nlohmann_json::nlohmann_json
         glm::glm-header-only)

set_target_properties(arcadia_foundation PROPERTIES FOLDER "engine")

option(ARCADIA_BUILD_FOUNDATION_SMOKE "Build the foundation ring smoke test" ON)

if(ARCADIA_BUILD_FOUNDATION_SMOKE)
  add_executable(foundation_smoke smoke/foundation_smoke.cpp)
  target_link_libraries(foundation_smoke PRIVATE arcadia_app)
  target_compile_features(foundation_smoke PRIVATE cxx_std_20)
  set_target_properties(foundation_smoke PROPERTIES FOLDER "engine/test")
endif()
