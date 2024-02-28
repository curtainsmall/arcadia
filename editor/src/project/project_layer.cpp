#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/util/conditional.hpp"
#include"resource/component/camera_component/camera_component.hpp"

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

void arcadia::project_layer::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::create_project>(ARCADIA_BIND_MEMBER_FN(_on_create_project))
        .bind_handler<arcadia::event::select_scene>(ARCADIA_BIND_MEMBER_FN(_on_select_scene))
        .bind_handler<arcadia::event::delete_scene>(ARCADIA_BIND_MEMBER_FN(_on_delete_scene))
        .bind_handler<arcadia::event::create_scene>(ARCADIA_BIND_MEMBER_FN(_on_create_scene))
        .dispatch();
}

void arcadia::project_layer::on_update(delta_time_type delta_time)
{}

void arcadia::project_layer::_on_create_project(arcadia::event::create_project& e)
{

    // TODO: Close current project before create new one

    const auto& [name, filepath_str] = e.data_tuple;
    _project_uptr = std::make_unique<arcadia::project>(name);
    _project_uptr->filepath = filepath_str.size() ? arcadia::to_filepath(filepath_str) : arcadia::to_filepath("./");

    arcadia::event_queue::instance()
        .signal<arcadia::event::project_built>(_project_uptr.get());
}

void arcadia::project_layer::_on_create_scene(arcadia::event::create_scene& e)
{
    const auto& [name, as_current] = e.data_tuple;
    auto& scene = _project_uptr->scene_umap.try_emplace(
        name,
        name
    ).first->second;

    auto entity = scene.create_entity("default_camera");

    auto& camera_comp = scene.emplace_component<arcadia::camera_component>(entity);
    camera_comp.pos ={ 0,0,10 };

    if(as_current)
    {
        _project_uptr->active_scene_ptr = &scene;
    }
}

void arcadia::project_layer::_on_select_scene(arcadia::event::select_scene& e)
{
    const auto& [name] = e.data_tuple;
    _project_uptr->active_scene_ptr = &_project_uptr->scene_umap.at(name);
}

void arcadia::project_layer::_on_delete_scene(arcadia::event::delete_scene& e)
{
    _project_uptr->scene_umap.erase(_project_uptr->active_scene_ptr->name);
    _project_uptr->active_scene_ptr = nullptr;
}

