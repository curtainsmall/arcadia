#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/file/pfd.hpp"
#include"core/log/log.hpp"
#include"core/util/conditional.hpp"
#include"core/util/hash.hpp"
#include"function/render/opengl/gl_renderer.hpp"
#include"resource/component/camera_component/camera_component.hpp"

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
        .bind_handler<arcadia::event::open_project>(ARCADIA_BIND_MEMBER_FN(_on_open_project))
        .bind_handler<arcadia::event::save_project>(ARCADIA_BIND_MEMBER_FN(_on_save_project))
        .bind_handler<arcadia::event::save_project_as>(ARCADIA_BIND_MEMBER_FN(_on_save_project_as))
        .bind_handler<arcadia::event::close_project>(ARCADIA_BIND_MEMBER_FN(_on_close_project))
        .bind_handler<arcadia::event::select_scene>(ARCADIA_BIND_MEMBER_FN(_on_select_scene))
        .bind_handler<arcadia::event::delete_scene>(ARCADIA_BIND_MEMBER_FN(_on_delete_scene))
        .bind_handler<arcadia::event::create_scene>(ARCADIA_BIND_MEMBER_FN(_on_create_scene))
        .dispatch();
}

void arcadia::project_layer::on_update(delta_time_type delta_time)
{}

void arcadia::project_layer::_save_project()
{
    ARCADIA_ASSERT(_project_uptr);

    nlohmann::json json{
        {"name",_project_uptr->get_name()},
        {"scene", nlohmann::json::object()},
        {"active_scene_name", _project_uptr->active_scene_ptr ? _project_uptr->active_scene_ptr->get_name() : ""s}
    };
    for(const auto& [name, scene] : _project_uptr->scene_umap)
    {
        json
            .at("scene")
            .push_back(
                { name,scene.to_json() }
        );
    }

    auto ofs = arcadia::file::create_ofstream(_project_filepath);
    ofs << std::setw(4) << json;

    arcadia::log::debug("Project saved");
}

void arcadia::project_layer::_load_project()
{
    ARCADIA_ASSERT(!_project_uptr);

    auto ifs = arcadia::file::create_ifstream(_project_filepath);
    auto json = nlohmann::json::parse(ifs);

    _project_uptr = std::make_unique<arcadia::project>(json.at("name"));

    for(const auto& [key, json_scene] : json.at("scene").items())
    {
        _project_uptr->scene_umap.try_emplace(key, json_scene);
    }

    auto& active_scene_name = json.at("active_scene_name");
    _project_uptr->active_scene_ptr = active_scene_name.size() ? &_project_uptr->scene_umap.at(active_scene_name) : nullptr;

    arcadia::log::debug("Project loaded");
}

void arcadia::project_layer::_on_create_project(arcadia::event::create_project& e)
{
    if(_project_uptr)
    {
        if(_project_filepath.empty())
        {
            _project_filepath = pfd::save_file{
                "Save as"
            }.result();
            if(_project_filepath.empty())
            {
                return;
            }
        }
        _save_project();
        _project_uptr.reset();
    }

    const auto& [name, filepath_str] = e.data_tuple;
    _project_uptr = std::make_unique<arcadia::project>(name);
    _project_filepath = filepath_str.size() ? arcadia::to_filepath(filepath_str) : std::filesystem::path{};

    arcadia::event_queue::instance()
        .signal<arcadia::event::project_built>(_project_uptr.get());
}

void arcadia::project_layer::_on_open_project(arcadia::event::open_project& e)
{
    if(_project_uptr)
    {
        if(_project_filepath.empty())
        {
            _project_filepath = pfd::save_file{
                "Save as"
            }.result();
            if(_project_filepath.empty())
            {
                return;
            }
        }
        _save_project();
        _project_uptr.reset();
    }

    auto filepathes = pfd::open_file{
        "Open"
    }.result();
    _project_filepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_project_filepath.empty())
    {
        return;
    }
    _load_project();
}

void arcadia::project_layer::_on_save_project(arcadia::event::save_project& e)
{
    ARCADIA_ASSERT(_project_uptr);

    if(_project_filepath.empty())
    {
        _project_filepath = pfd::save_file{
            "Save as"
        }.result();
        if(_project_filepath.empty())
        {
            return;
        }
    }
    _save_project();
}

void arcadia::project_layer::_on_save_project_as(arcadia::event::save_project_as& e)
{
    ARCADIA_ASSERT(_project_uptr);

    _project_filepath = pfd::save_file{
        "Save as"
    }.result();
    if(_project_filepath.empty())
    {
        return;
    }
    _save_project();
}

void arcadia::project_layer::_on_close_project(arcadia::event::close_project& e)
{
    ARCADIA_ASSERT(_project_uptr);

    if(_project_uptr->is_modified())
    {
        if(_project_filepath.empty())
        {
            _project_filepath = pfd::save_file{
                "Save as"
            }.result();
            if(_project_filepath.empty())
            {
                return;
            }
        }
        _save_project();
    }
    _project_uptr.reset();
}

void arcadia::project_layer::_on_create_scene(arcadia::event::create_scene& e)
{
    const auto& [name, as_current] = e.data_tuple;
    auto& scene = _project_uptr->scene_umap.try_emplace(
        name,
        name
    ).first->second;

    auto camera_entity = scene.create_entity("default_camera");

    auto& camera_comp = scene.emplace_component<arcadia::camera_component>(camera_entity);
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
    _project_uptr->scene_umap.erase(_project_uptr->active_scene_ptr->get_name());
    _project_uptr->active_scene_ptr = nullptr;
}

