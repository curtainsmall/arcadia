set(imguizmo_SOURCE_DIR_ "${CMAKE_CURRENT_SOURCE_DIR}/imguizmo")

file(GLOB imguizmo_sources CONFIGURE_DEPENDS "${imguizmo_SOURCE_DIR_}/*.cpp"
     "${imguizmo_SOURCE_DIR_}/*.h")
add_library(imguizmo STATIC "${imguizmo_sources}")
target_include_directories(imguizmo
                           PUBLIC $<BUILD_INTERFACE:${imguizmo_SOURCE_DIR_}>)
target_include_directories(imguizmo PUBLIC "${imguizmo_SOURCE_DIR_}"
                                           "${imgui_SOURCE_DIR_}")
target_link_directories(imguizmo PUBLIC imgui)
