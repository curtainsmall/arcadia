#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/file/pfd_header.hpp"
#include"core/hash.hpp"
#include"core/log/log.hpp"
#include"core/memento/memento.hpp"
#include"function/render/opengl/gl_renderer.hpp"
#include"function/window/window_events.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

#include"editor/editor_context.hpp"

ProjectLayer::ProjectLayer():
    iLayer("project")
{
    const auto& app_config = AppConfig::instance();
    auto& event_queue = EventQueue::instance();

    match<void>(
        app_config.graphic_api,
        [&](const graphic_api::Opengl&)
    {
        static OpenglContext gl_context{};
        _renderer = std::make_shared<GlRenderer>(app_config.working_directory / to_filepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
    event_queue.signal<event::RendererBuilt>(_renderer);

    _physics_simulator = std::make_shared<PhysicsSimulator>();
    event_queue.signal<event::PhysicsSimulatorBuilt>(_physics_simulator);
}

ProjectLayer::~ProjectLayer()
{
    auto& event_queue = EventQueue::instance();
    event_queue.signal<event::RendererUnbuilt>();
    event_queue.signal<event::PhysicsSimulatorUnbuilt>();
}

void ProjectLayer::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<event::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_on_window_should_close))
        .dispatch<event::CreateProject>(ACDA_BIND_MEMBER_FN(_on_create_project))
        .dispatch<event::OpenProject>(ACDA_BIND_MEMBER_FN(_on_open_project))
        .dispatch<event::SaveProject>(ACDA_BIND_MEMBER_FN(_on_save_project))
        .dispatch<event::SaveProjectAs>(ACDA_BIND_MEMBER_FN(_on_save_project_as))
        .dispatch<event::CloseProject>(ACDA_BIND_MEMBER_FN(_on_close_project))
        .dispatch<event::ProjectSaved>(ACDA_BIND_MEMBER_FN(_on_project_saved))
        .dispatch<event::CreateScene>(ACDA_BIND_MEMBER_FN(_on_create_scene))
        .dispatch<event::SelectScene>(ACDA_BIND_MEMBER_FN(_on_select_scene))
        .dispatch<event::CloseScene>(ACDA_BIND_MEMBER_FN(_on_close_scene))
        .dispatch<event::DeleteScene>(ACDA_BIND_MEMBER_FN(_on_delete_scene))
        .dispatch<event::NewEntity>(ACDA_BIND_MEMBER_FN(_OnNewEntity))
        .dispatch<event::RenameEntity>(ACDA_BIND_MEMBER_FN(_on_rename_entity))
        .dispatch<event::DeleteEntity>(ACDA_BIND_MEMBER_FN(_on_delete_entity))
        .dispatch<event::AddComponent>(ACDA_BIND_MEMBER_FN(_on_add_component))
        .dispatch<event::RemoveComponent>(ACDA_BIND_MEMBER_FN(_on_remove_component))
        .result();
}

void ProjectLayer::on_update()
{}

auto ProjectLayer::_assert_and_get_scene() -> Scene&
{
    ACDA_ASSERT(_project);
    ACDA_ASSERT(_project->has_active_scene());

    return _project->get_active_scene();
}

void ProjectLayer::_save_project()
{
    ACDA_ASSERT(_project);

    auto json = _project->to_json();

    auto ofs = File::create_ofstream(_project_component);
    ofs << std::setw(4) << json;

    EventQueue::instance().signal<event::ProjectSaved>();
}

void ProjectLayer::_load_project()
{
    ACDA_ASSERT(!_project);

    auto ifs = File::create_ifstream(_project_component);
    auto json = nlohmann::json::parse(ifs);

    _project = std::make_shared<Project>(json);
    EventQueue::instance().signal<event::ProjectLoaded>();
}

void ProjectLayer::_on_window_should_close(event::WindowShouldClose& e)
{
    auto& event_queue = EventQueue::instance();

    auto main_window_layer = EditorContext::instance().main_window_layer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == main_window_layer.get() && _project)
    {
        auto& memento_list = MementoList::instance();
        if(memento_list.size())
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
                    event_queue.signal<event::WindowCloseCanceled>(p_wnd);
                    return;
                }
                case pfd::button::yes:
                {
                    if(_project_component.empty())
                    {
                        _project_component = pfd::save_file{
                            "Save as"
                        }.result();
                        if(_project_component.empty())
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
        _project.reset();
        EventQueue::instance()
            .signal<event::ProjectUnbuilt>();

    }
}

void ProjectLayer::_on_create_project(event::CreateProject& e)
{
    if(_project)
    {
        auto res = pfd::message{
            "Unsaved Project",
            "The current project is not saved, do you want to save it?",
            pfd::choice::yes_no_cancel
        }.result();

        switch(res)
        {
            case pfd::button::cancel:
                return;
            case pfd::button::yes:
            {
                if(_project_component.empty())
                {
                    _project_component = pfd::save_file{
                                        "Save as"
                    }.result();
                    if(_project_component.empty())
                    {
                        return;
                    }
                }
                _save_project();
                break;
            }
            default:
                break;
        }
        _project.reset();
    }

    const auto& [name, filepath_str] = e.data_tuple;
    _project = std::make_shared<Project>(name);
    _project_component = filepath_str.size() ? to_filepath(filepath_str) : std::filesystem::path{};

    EventQueue::instance()
        .signal<event::ProjectBuilt>(_project);
}

void ProjectLayer::_on_open_project(event::OpenProject& e)
{
    if(_project)
    {
        auto res = pfd::message{
            "Unsaved Project",
            "The current project is not saved, do you want to save it?",
            pfd::choice::yes_no_cancel
        }.result();

        switch(res)
        {
            case pfd::button::cancel:
                return;
            case pfd::button::yes:
            {
                if(_project_component.empty())
                {
                    _project_component = pfd::save_file{
                                        "Save as"
                    }.result();
                    if(_project_component.empty())
                    {
                        return;
                    }
                }
                _save_project();
                break;
            }
            default:
                break;
        }
        _project.reset();
    }

    auto filepathes = pfd::open_file{
        "Open",
        "",
        std::vector<std::string>{"Arcadia Project",std::format("*{}",Project::project_extension_str)}
    }.result();
    _project_component = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_project_component.empty())
    {
        return;
    }
    if(_project_component.extension() != Project::project_extension_str)
    {
        pfd::message msg{
            "Open Project",
            std::format("Arcadia project must ends with extension \"{}\" while {} does not",Project::project_extension_str,_project_component.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _load_project();
    EventQueue::instance()
        .signal<event::ProjectBuilt>(_project);
}

void ProjectLayer::_on_save_project(event::SaveProject& e)
{
    ACDA_ASSERT(_project);

    if(_project_component.empty())
    {
        _project_component = pfd::save_file{
            "Save as"
        }.result();
        if(_project_component.empty())
        {
            return;
        }
    }
    _save_project();
}

void ProjectLayer::_on_save_project_as(event::SaveProjectAs& e)
{
    ACDA_ASSERT(_project);

    _project_component = pfd::save_file{
        "Save as"
    }.result();
    if(_project_component.empty())
    {
        return;
    }
    _save_project();
}

void ProjectLayer::_on_close_project(event::CloseProject& e)
{
    ACDA_ASSERT(_project);

    auto& event_queue = EventQueue::instance();

    auto& memento_list = MementoList::instance();
    if(memento_list.size())
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
                if(_project_component.empty())
                {
                    _project_component = pfd::save_file{
                        "Save as"
                    }.result();
                    if(_project_component.empty())
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
    _project.reset();
    EventQueue::instance()
        .signal<event::ProjectUnbuilt>();
}

void ProjectLayer::_on_project_saved(event::ProjectSaved& e)
{
    MementoList::instance().clear();
}

void ProjectLayer::_on_create_scene(event::CreateScene& e)
{
    ACDA_ASSERT(_project);

    const auto& [name, as_current] = e.data_tuple;
    auto& scene = _project->scene_sptr_storage.try_emplace(
        name,
        std::make_shared<Scene>(name)
    ).first->second;

    if(as_current)
    {
        _project->set_active_scene(name);
    }
}

void ProjectLayer::_on_select_scene(event::SelectScene& e)
{
    ACDA_ASSERT(_project);

    const auto& [name] = e.data_tuple;
    _project->set_active_scene(name);
}

void ProjectLayer::_on_close_scene(event::CloseScene& e)
{
    ACDA_ASSERT(_project);

    _project->set_active_scene();
}

void ProjectLayer::_on_delete_scene(event::DeleteScene& e)
{
    ACDA_ASSERT(_project);
    ACDA_ASSERT(_project->has_active_scene());

    const auto& scene_name = _project->get_active_scene().name;

    auto res = pfd::message{
        "Delete Scene",
        std::format("Do you want to delete scene: {}", scene_name)
    }.result();

    switch(res)
    {
        case pfd::button::ok:
        {
            _project->scene_sptr_storage.erase(scene_name);
            _project->set_active_scene();
            break;
        }
        case pfd::button::cancel:
        default:
        {
            break;
        }
    }

}

void ProjectLayer::_OnNewEntity(event::NewEntity& e)
{
    const auto& [type] = e.data_tuple;

    auto& scene = _assert_and_get_scene();

    std::string temp_name = "New Entity";
    std::string name = temp_name;
    int postfix{ 1 };
    while(scene.contains(name))
    {
        name = std::format("{} {}", temp_name, ++postfix);
    }

    scene.create(name, type);

    match<void>(
        type,
        "actor"s,
        [&]()
    {
        scene.emplace<ModelComponent>(name).snapshot();

        scene.emplace<PhysicsComponent>(name).snapshot();

        auto& transform_comp =  scene.emplace<TransformComponent>(name);
        transform_comp.snapshot();
        transform_comp.Flags |= transform_component_flags::UseRotation;
    },
        "camera"s,
        [&]()
    {
        scene.emplace<CameraComponent>(name).snapshot();

        auto& transform_comp = scene.emplace<TransformComponent>(name);
        transform_comp.snapshot();
        transform_comp.Flags |= transform_component_flags::UseDirection;
    },
        "light"s,
        [&]()
    {
        scene.emplace<LightComponent>(name).snapshot();

        auto& transform_comp = scene.emplace<TransformComponent>(name);
        transform_comp.snapshot();
        transform_comp.Flags |= transform_component_flags::UseDirection;
    }
    );
}

void ProjectLayer::_on_rename_entity(event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;

    _project->get_active_scene().rename(old_name, new_name);
}

void ProjectLayer::_on_delete_entity(event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    auto& scene = _assert_and_get_scene();
    scene.destroy(entity);
}

void ProjectLayer::_on_add_component(event::AddComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _assert_and_get_scene();

    match<void>(
        type_str,
        CameraComponent::get_type_str_static(),
        [&]()
    {
        scene.emplace<CameraComponent>(entity);
    },
        LightComponent::get_type_str_static(),
        [&]()
    {
        scene.emplace<LightComponent>(entity);
    },
        ModelComponent::get_type_str_static(),
        [&]()
    {
        scene.emplace<ModelComponent>(entity);
    },
        PhysicsComponent::get_type_str_static(),
        [&]()
    {
        scene.emplace<PhysicsComponent>(entity);
    }
    );
}

void ProjectLayer::_on_remove_component(event::RemoveComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _assert_and_get_scene();

    match<void>(
        type_str,
        CameraComponent::get_type_str_static(),
        [&]()
    {
        scene.remove<CameraComponent>(entity);
    },
        LightComponent::get_type_str_static(),
        [&]()
    {
        scene.remove<LightComponent>(entity);
    },
        ModelComponent::get_type_str_static(),
        [&]()
    {
        scene.remove<ModelComponent>(entity);
    },
        PhysicsComponent::get_type_str_static(),
        [&]()
    {
        scene.remove<PhysicsComponent>(entity);
    }
    );
}

