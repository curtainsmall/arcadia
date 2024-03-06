#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/file/pfd.hpp"
#include"core/log/log.hpp"
#include"core/util/conditional.hpp"
#include"core/util/hash.hpp"
#include"function/render/opengl/gl_renderer.hpp"
#include"function/window/window_events.hpp"
#include"resource/component/camera_component/camera_component.hpp"

#include"editor/editor_context.hpp"

arcadia::project_layer::project_layer():
    arcadia::layer_interface("project")
{
    const auto& app_config = arcadia::app_config::instance();

    arcadia::match<void>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl&)
    {
        static arcadia::opengl_context gl_context{};
        _renderer_sptr = std::make_shared<arcadia::gl_renderer>(app_config.working_directory / arcadia::to_filepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
}

void arcadia::project_layer::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::window_should_close>(ARCADIA_BIND_MEMBER_FN(_on_window_should_close))
        .dispatch<arcadia::event::create_project>(ARCADIA_BIND_MEMBER_FN(_on_create_project))
        .dispatch<arcadia::event::open_project>(ARCADIA_BIND_MEMBER_FN(_on_open_project))
        .dispatch<arcadia::event::save_project>(ARCADIA_BIND_MEMBER_FN(_on_save_project))
        .dispatch<arcadia::event::save_project_as>(ARCADIA_BIND_MEMBER_FN(_on_save_project_as))
        .dispatch<arcadia::event::close_project>(ARCADIA_BIND_MEMBER_FN(_on_close_project))
        .dispatch<arcadia::event::create_scene>(ARCADIA_BIND_MEMBER_FN(_on_create_scene))
        .dispatch<arcadia::event::select_scene>(ARCADIA_BIND_MEMBER_FN(_on_select_scene))
        .dispatch<arcadia::event::delete_scene>(ARCADIA_BIND_MEMBER_FN(_on_delete_scene))
        .dispatch<arcadia::event::create_entity>(ARCADIA_BIND_MEMBER_FN(_on_create_entity))
        .dispatch<arcadia::event::rename_entity>(ARCADIA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<arcadia::event::delete_entity>(ARCADIA_BIND_MEMBER_FN(_on_delete_entity))
        .result();
}

void arcadia::project_layer::on_update(delta_time_type delta_time)
{}

void arcadia::project_layer::_save_project()
{
    ARCADIA_ASSERT(_project_sptr);

    nlohmann::json json{
        {"name",_project_sptr->get_name()},
        {"scenes", nlohmann::json::array()},
        {"active_scene_name", _project_sptr->has_active_scene() ? _project_sptr->get_active_scene().get_name() : ""s}
    };
    for(const auto& [name, scene_sptr] : _project_sptr->scene_sptr_umap)
    {
        json
            .at("scenes")
            .push_back(
                scene_sptr->to_json()
            );
    }

    auto ofs = arcadia::file::create_ofstream(_project_filepath);
    ofs << std::setw(4) << json;

    arcadia::log::debug("Project saved");
}

void arcadia::project_layer::_load_project()
{
    ARCADIA_ASSERT(!_project_sptr);

    auto ifs = arcadia::file::create_ifstream(_project_filepath);
    auto json = nlohmann::json::parse(ifs);

    _project_sptr = std::make_shared<arcadia::project>(json.at("name"));

    for(const auto& json_scene : json.at("scenes"))
    {
        _project_sptr->scene_sptr_umap.try_emplace(json_scene.at("name"), std::make_shared<arcadia::scene>(json_scene));
    }

    auto& active_scene_name = json.at("active_scene_name");
    _project_sptr->set_active_scene(active_scene_name);

    arcadia::log::debug("Project loaded");
}

void arcadia::project_layer::_on_window_should_close(arcadia::event::window_should_close& e)
{
    auto& event_queue = arcadia::event_queue::instance();

    auto main_window_layer_sptr = arcadia::editor_context::instance().main_window_layer_wptr.lock();

    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == main_window_layer_sptr.get() && _project_sptr)
    {
        if(_project_sptr->is_modified())
        {
            auto res = pfd::message{
                "Unsaved",
                "Do you want to save changes in project?",
                pfd::choice::yes_no_cancel,
                pfd::icon::question
            }.result();

            switch(res)
            {
                case pfd::button::cancel:
                {
                    event_queue.signal<arcadia::event::window_close_canceled>(wnd_ptr);
                    return;
                }
                case pfd::button::yes:
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
                    break;
                }
                case pfd::button::no:
                    break;
            }

        }
        _project_sptr.reset();
        arcadia::event_queue::instance()
            .signal<arcadia::event::project_unbuilt>();
    }
}

void arcadia::project_layer::_on_create_project(arcadia::event::create_project& e)
{
    if(_project_sptr)
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
        _project_sptr.reset();
    }

    const auto& [name, filepath_str] = e.data_tuple;
    _project_sptr = std::make_shared<arcadia::project>(name);
    _project_sptr->set_modified(true);
    _project_filepath = filepath_str.size() ? arcadia::to_filepath(filepath_str) : std::filesystem::path{};

    arcadia::event_queue::instance()
        .signal<arcadia::event::project_built>(_project_sptr);
}

void arcadia::project_layer::_on_open_project(arcadia::event::open_project& e)
{
    if(_project_sptr)
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
        _project_sptr.reset();
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
    _project_sptr->set_modified(false, true);
    arcadia::event_queue::instance()
        .signal<arcadia::event::project_built>(_project_sptr);
}

void arcadia::project_layer::_on_save_project(arcadia::event::save_project& e)
{
    ARCADIA_ASSERT(_project_sptr);

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
    _project_sptr->set_modified(false, true);
}

void arcadia::project_layer::_on_save_project_as(arcadia::event::save_project_as& e)
{
    ARCADIA_ASSERT(_project_sptr);

    _project_filepath = pfd::save_file{
        "Save as"
    }.result();
    if(_project_filepath.empty())
    {
        return;
    }
    _save_project();
    _project_sptr->set_modified(false, true);
}

void arcadia::project_layer::_on_close_project(arcadia::event::close_project& e)
{
    ARCADIA_ASSERT(_project_sptr);

    auto& event_queue = arcadia::event_queue::instance();

    if(_project_sptr->is_modified())
    {
        auto res = pfd::message{
                        "Unsaved",
                        "Do you want to save changes in project?",
                        pfd::choice::yes_no_cancel,
                        pfd::icon::question
        }.result();

        switch(res)
        {
            case pfd::button::cancel:
            {
                return;
            }
            case pfd::button::yes:
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
                break;
            }
            case pfd::button::no:
                break;
        }
    }
    _project_sptr.reset();
    arcadia::event_queue::instance()
        .signal<arcadia::event::project_unbuilt>();
}

void arcadia::project_layer::_on_create_scene(arcadia::event::create_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);

    const auto& [name, as_current] = e.data_tuple;
    auto& scene_sptr = _project_sptr->scene_sptr_umap.try_emplace(
        name,
        std::make_shared<arcadia::scene>(name)
    ).first->second;

    auto camera_entity = scene_sptr->create_entity("default_camera");

    auto& camera_comp = scene_sptr->emplace_component<arcadia::camera_component>(camera_entity);
    camera_comp.pos ={ 0,0,10 };

    if(as_current)
    {
        _project_sptr->set_active_scene(name);
    }
}

void arcadia::project_layer::_on_select_scene(arcadia::event::select_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);

    const auto& [name] = e.data_tuple;
    _project_sptr->set_active_scene(name);
}

void arcadia::project_layer::_on_delete_scene(arcadia::event::delete_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);

    if(_project_sptr->has_active_scene())
    {
        _project_sptr->scene_sptr_umap.erase(_project_sptr->get_active_scene().get_name());
        _project_sptr->set_active_scene();
    }
    else
    {
        pfd::message{
            "Delete scene",
            "There is no current scene",
            pfd::choice::ok,
            pfd::icon::warning
        };
    }
}

void arcadia::project_layer::_on_create_entity(arcadia::event::create_entity& e)
{
    ARCADIA_ASSERT(_project_sptr);
    ARCADIA_ASSERT(_project_sptr->has_active_scene());

    const auto& [name] = e.data_tuple;

    auto& scene = _project_sptr->get_active_scene();
    auto entity = scene.create_entity(name);

}

void arcadia::project_layer::_on_rename_entity(arcadia::event::rename_entity& e)
{
    ARCADIA_ASSERT(_project_sptr);
    ARCADIA_ASSERT(_project_sptr->has_active_scene());

    const auto& [old_name, new_name] = e.data_tuple;

    if(!_project_sptr->get_active_scene().rename_entity(old_name, new_name))
    {
        pfd::message{
            "Rename Entity",
            std::format("Failed to rename {} to {}, because the new name is already used",old_name,new_name),
            pfd::choice::ok,
            pfd::icon::info
        };
    }
}

void arcadia::project_layer::_on_delete_entity(arcadia::event::delete_entity& e)
{
    ARCADIA_ASSERT(_project_sptr);
    ARCADIA_ASSERT(_project_sptr->has_active_scene());

    const auto& [entity] = e.data_tuple;
    auto& scene = _project_sptr->get_active_scene();
    scene.destroy_entity(entity);
}

