# app ring
#
# The host and the two seams it talks to. No legacy dependency: this ring only
# knows foundation, so it stays compilable and testable on its own.

add_library(arcadia_app STATIC
  runtime_config.cpp
  engine.cpp)

target_include_directories(arcadia_app PUBLIC "${ENGINE_ROOT_DIR}/source")

target_compile_features(arcadia_app PUBLIC cxx_std_20)

target_link_libraries(arcadia_app PUBLIC arcadia_foundation)

set_target_properties(arcadia_app PROPERTIES FOLDER "engine")

