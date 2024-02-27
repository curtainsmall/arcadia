#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/util/conditional.hpp"
#include"function/render/opengl/gl_renderer.hpp"

arcadia::project_layer::project_layer():
    arcadia::layer_interface("project")
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl&)
    {
        static arcadia::opengl_context gl_context{};
        _renderer_uptr = std::make_unique<arcadia::gl_renderer>(app_config.working_directory / arcadia::to_filepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
}

auto arcadia::project_layer::on_event(const arcadia::event_base& event) -> bool
{
    return false;
}

void arcadia::project_layer::on_update(delta_time_type delta_time)
{}
