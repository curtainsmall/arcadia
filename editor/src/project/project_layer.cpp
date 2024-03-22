#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/file/pfd_header.hpp"
#include"core/hash.hpp"
#include"core/log/log.hpp"
#include"function/render/opengl/gl_renderer.hpp"
#include"function/window/window_events.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"

#include"editor/editor_context.hpp"

arcadia::project_layer::project_layer():
    arcadia::layer_interface("project")
{
    const auto& app_config = arcadia::app_config::instance();
    auto& event_queue = arcadia::event_queue::instance();

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
    event_queue.signal<arcadia::event::renderer_built>(_renderer_sptr);

    _physics_simulator_sptr = std::make_shared<arcadia::physics_simulator>();
    event_queue.signal<arcadia::event::physics_simulator_built>(_physics_simulator_sptr);
}

arcadia::project_layer::~project_layer()
{
    auto& event_queue = arcadia::event_queue::instance();
    event_queue.signal<arcadia::event::renderer_unbuilt>();
    event_queue.signal<arcadia::event::physics_simulator_unbuilt>();
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
        .dispatch<arcadia::event::close_scene>(ARCADIA_BIND_MEMBER_FN(_on_close_scene))
        .dispatch<arcadia::event::delete_scene>(ARCADIA_BIND_MEMBER_FN(_on_delete_scene))
        .dispatch<arcadia::event::new_entity>(ARCADIA_BIND_MEMBER_FN(_on_new_entity))
        .dispatch<arcadia::event::rename_entity>(ARCADIA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<arcadia::event::delete_entity>(ARCADIA_BIND_MEMBER_FN(_on_delete_entity))
        .dispatch<arcadia::event::add_component>(ARCADIA_BIND_MEMBER_FN(_on_add_component))
        .dispatch<arcadia::event::remove_component>(ARCADIA_BIND_MEMBER_FN(_on_remove_component))
        .result();
}

void arcadia::project_layer::on_update()
{}

auto arcadia::project_layer::_get_scene_or_assert() -> arcadia::scene&
{
    ARCADIA_ASSERT(_project_sptr);
    ARCADIA_ASSERT(_project_sptr->has_active_scene());

    return _project_sptr->get_active_scene();
}

void arcadia::project_layer::_save_project()
{
    ARCADIA_ASSERT(_project_sptr);

    auto json = _project_sptr->to_json();

    auto ofs = arcadia::file::create_ofstream(_project_filepath);
    ofs << std::setw(4) << json;
}

void arcadia::project_layer::_load_project()
{
    ARCADIA_ASSERT(!_project_sptr);

    auto ifs = arcadia::file::create_ifstream(_project_filepath);
    auto json = nlohmann::json::parse(ifs);

    _project_sptr = std::make_shared<arcadia::project>(json);
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
    }

    auto filepathes = pfd::open_file{
        "Open",
        "",
        std::vector<std::string>{"Arcadia Project","*.acdaprj"}
    }.result();
    _project_filepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_project_filepath.empty())
    {
        return;
    }
    if(_project_filepath.extension() != ".acdaprj")
    {
        pfd::message{
            "Open Project",
            std::format("Arcadia project must ends with extension \".arcaprj\" while {} does not",_project_filepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _project_sptr.reset();
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

    if(as_current)
    {
        _project_sptr->set_active_scene(name);
    }
}

void arcadia::project_layer::_on_select_scene(arcadia::event::select_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);

    const auto& [name] = e.data_tuple;
    auto& active_scene_wptr = _project_sptr->set_active_scene(name);
}

void arcadia::project_layer::_on_close_scene(arcadia::event::close_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);

    _project_sptr->set_active_scene();
}

void arcadia::project_layer::_on_delete_scene(arcadia::event::delete_scene& e)
{
    ARCADIA_ASSERT(_project_sptr);
    ARCADIA_ASSERT(_project_sptr->has_active_scene());

    const auto& scene_name = _project_sptr->get_active_scene().get_name();

    auto res = pfd::message{
        "Delete Scene",
        std::format("Do you want to delete scene: {}", scene_name)
    }.result();

    switch(res)
    {
        case pfd::button::ok:
        {
            _project_sptr->scene_sptr_umap.erase(scene_name);
            _project_sptr->set_active_scene();
            break;
        }
        case pfd::button::cancel:
        default:
        {
            break;
        }
    }

}

void arcadia::project_layer::_on_new_entity(arcadia::event::new_entity& e)
{
    auto& scene = _get_scene_or_assert();

    std::string name = "New Entity";
    std::string final_name = name;
    int postfix{ 1 };
    while(scene.contains_entity(final_name))
    {
        final_name = std::format("{} {}", name, ++postfix);
    }

    ARCADIA_DISCARD(scene.create_entity(final_name));
}

void arcadia::project_layer::_on_rename_entity(arcadia::event::rename_entity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;

    auto& scene = _get_scene_or_assert();
    if(!scene.rename_entity(old_name, new_name))
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
    const auto& [entity] = e.data_tuple;
    auto& scene = _get_scene_or_assert();
    scene.destroy_entity(entity);
}

void arcadia::project_layer::_on_add_component(arcadia::event::add_component& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _get_scene_or_assert();

    arcadia::match<void>(
        type_str,
        arcadia::camera_component::get_type_str_static(),
        [&]()
    {
        scene.emplace_component<arcadia::camera_component>(entity);
    },
        arcadia::light_component::get_type_str_static(),
        [&]()
    {
        scene.emplace_component<arcadia::light_component>(entity);
    },
        arcadia::model_component::get_type_str_static(),
        [&]()
    {
        scene.emplace_component<arcadia::model_component>(entity);
    },
        arcadia::physics_component::get_type_str_static(),
        [&]()
    {
        scene.emplace_component<arcadia::physics_component>(entity);
    }
    );
}

void arcadia::project_layer::_on_remove_component(arcadia::event::remove_component& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _get_scene_or_assert();

    arcadia::match<void>(
        type_str,
        arcadia::camera_component::get_type_str_static(),
        [&]()
    {
        scene.remove_conponent<arcadia::camera_component>(entity);
    },
        arcadia::light_component::get_type_str_static(),
        [&]()
    {
        scene.remove_conponent<arcadia::light_component>(entity);
    },
        arcadia::model_component::get_type_str_static(),
        [&]()
    {
        scene.remove_conponent<arcadia::model_component>(entity);
    },
        arcadia::physics_component::get_type_str_static(),
        [&]()
    {
        scene.remove_conponent<arcadia::physics_component>(entity);
    }
    );
}

