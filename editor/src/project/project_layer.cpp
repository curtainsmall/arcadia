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
    const auto& app_config = AppConfig::Instance();
    auto& event_queue = EventQueue::Instance();

    Match<void>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl&)
    {
        static OpenglContext gl_context{};
        _Renderer = std::make_shared<GlRenderer>(app_config.WorkingDirectory / ToFilepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
    event_queue.Signal<Event::RendererBuilt>(_Renderer);

    _PhysicsSimulator = std::make_shared<PhysicsSimulator>();
    event_queue.Signal<Event::PhysicsSimulatorBuilt>(_PhysicsSimulator);
}

ProjectLayer::~ProjectLayer()
{
    auto& event_queue = EventQueue::Instance();
    event_queue.Signal<Event::RendererUnbuilt>();
    event_queue.Signal<Event::PhysicsSimulatorUnbuilt>();
}

void ProjectLayer::OnEvent(EventBase& event)
{
    EventDispatcher{ event }
        .Dispatch<Event::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Event::CreateProject>(ACDA_BIND_MEMBER_FN(_OnCreateProject))
        .Dispatch<Event::OpenProject>(ACDA_BIND_MEMBER_FN(_OnOpenProject))
        .Dispatch<Event::SaveProject>(ACDA_BIND_MEMBER_FN(_OnSaveProject))
        .Dispatch<Event::SaveProjectAs>(ACDA_BIND_MEMBER_FN(_OnSaveProjectAs))
        .Dispatch<Event::CloseProject>(ACDA_BIND_MEMBER_FN(_OnCloseProject))
        .Dispatch<Event::ProjectSaved>(ACDA_BIND_MEMBER_FN(_OnProjectSaved))
        .Dispatch<Event::CreateScene>(ACDA_BIND_MEMBER_FN(_OnCreateScene))
        .Dispatch<Event::SelectScene>(ACDA_BIND_MEMBER_FN(_OnSelectScene))
        .Dispatch<Event::CloseScene>(ACDA_BIND_MEMBER_FN(_OnCloseScene))
        .Dispatch<Event::DeleteScene>(ACDA_BIND_MEMBER_FN(_OnDeleteScene))
        .Dispatch<Event::NewEntity>(ACDA_BIND_MEMBER_FN(_OnNewEntity))
        .Dispatch<Event::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Event::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Event::AddComponent>(ACDA_BIND_MEMBER_FN(_OnAddComponent))
        .Dispatch<Event::RemoveComponent>(ACDA_BIND_MEMBER_FN(_OnRemoveComponent))
        .Result();
}

void ProjectLayer::OnUpdate()
{}

auto ProjectLayer::_AssertAndGetScene() -> Scene&
{
    ACDA_ASSERT(_Project);
    ACDA_ASSERT(_Project->HasActiveScene());

    return _Project->GetActiveScene();
}

void ProjectLayer::_SaveProject()
{
    ACDA_ASSERT(_Project);

    auto json = _Project->ToJson();

    auto ofs = File::CreateOfstream(_ProjectFilepath);
    ofs << std::setw(4) << json;

    EventQueue::Instance().Signal<Event::ProjectSaved>();
}

void ProjectLayer::_LoadProject()
{
    ACDA_ASSERT(!_Project);

    auto ifs = File::CreateIfstream(_ProjectFilepath);
    auto json = nlohmann::json::parse(ifs);

    _Project = std::make_shared<Project>(json);
    EventQueue::Instance().Signal<Event::ProjectLoaded>();
}

void ProjectLayer::_OnWindowShouldClose(Event::WindowShouldClose& e)
{
    auto& event_queue = EventQueue::Instance();

    auto main_window_layer = EditorContext::Instance().MainWindowLayer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == main_window_layer.get() && _Project)
    {
        auto& memento_list = MementoList::Instance();
        if(memento_list.Size())
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
                    event_queue.Signal<Event::WindowCloseCanceled>(p_wnd);
                    return;
                }
                case pfd::button::yes:
                {
                    if(_ProjectFilepath.empty())
                    {
                        _ProjectFilepath = pfd::save_file{
                            "Save as"
                        }.result();
                        if(_ProjectFilepath.empty())
                        {
                            return;
                        }
                    }
                    _SaveProject();

                    break;
                }
                case pfd::button::no:
                    break;
            }
        }
        _Project.reset();
        EventQueue::Instance()
            .Signal<Event::ProjectUnbuilt>();

    }
}

void ProjectLayer::_OnCreateProject(Event::CreateProject& e)
{
    if(_Project)
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
                if(_ProjectFilepath.empty())
                {
                    _ProjectFilepath = pfd::save_file{
                                        "Save as"
                    }.result();
                    if(_ProjectFilepath.empty())
                    {
                        return;
                    }
                }
                _SaveProject();
                break;
            }
            default:
                break;
        }
        _Project.reset();
    }

    const auto& [name, filepath_str] = e.data_tuple;
    _Project = std::make_shared<Project>(name);
    _ProjectFilepath = filepath_str.size() ? ToFilepath(filepath_str) : std::filesystem::path{};

    EventQueue::Instance()
        .Signal<Event::ProjectBuilt>(_Project);
}

void ProjectLayer::_OnOpenProject(Event::OpenProject& e)
{
    if(_Project)
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
                if(_ProjectFilepath.empty())
                {
                    _ProjectFilepath = pfd::save_file{
                                        "Save as"
                    }.result();
                    if(_ProjectFilepath.empty())
                    {
                        return;
                    }
                }
                _SaveProject();
                break;
            }
            default:
                break;
        }
        _Project.reset();
    }

    auto filepathes = pfd::open_file{
        "Open",
        "",
        std::vector<std::string>{"Arcadia Project",std::format("*{}",Project::ProjectExtensionStr)}
    }.result();
    _ProjectFilepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_ProjectFilepath.empty())
    {
        return;
    }
    if(_ProjectFilepath.extension() != Project::ProjectExtensionStr)
    {
        pfd::message msg{
            "Open Project",
            std::format("Arcadia project must ends with extension \"{}\" while {} does not",Project::ProjectExtensionStr,_ProjectFilepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _LoadProject();
    EventQueue::Instance()
        .Signal<Event::ProjectBuilt>(_Project);
}

void ProjectLayer::_OnSaveProject(Event::SaveProject& e)
{
    ACDA_ASSERT(_Project);

    if(_ProjectFilepath.empty())
    {
        _ProjectFilepath = pfd::save_file{
            "Save as"
        }.result();
        if(_ProjectFilepath.empty())
        {
            return;
        }
    }
    _SaveProject();
}

void ProjectLayer::_OnSaveProjectAs(Event::SaveProjectAs& e)
{
    ACDA_ASSERT(_Project);

    _ProjectFilepath = pfd::save_file{
        "Save as"
    }.result();
    if(_ProjectFilepath.empty())
    {
        return;
    }
    _SaveProject();
}

void ProjectLayer::_OnCloseProject(Event::CloseProject& e)
{
    ACDA_ASSERT(_Project);

    auto& event_queue = EventQueue::Instance();

    auto& memento_list = MementoList::Instance();
    if(memento_list.Size())
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
                if(_ProjectFilepath.empty())
                {
                    _ProjectFilepath = pfd::save_file{
                        "Save as"
                    }.result();
                    if(_ProjectFilepath.empty())
                    {
                        return;
                    }
                }
                _SaveProject();
                break;
            }
            case pfd::button::no:
                break;
        }
    }
    _Project.reset();
    EventQueue::Instance()
        .Signal<Event::ProjectUnbuilt>();
}

void ProjectLayer::_OnProjectSaved(Event::ProjectSaved& e)
{
    MementoList::Instance().Clear();
}

void ProjectLayer::_OnCreateScene(Event::CreateScene& e)
{
    ACDA_ASSERT(_Project);

    const auto& [name, as_current] = e.data_tuple;
    auto& scene = _Project->SceneSptrStorage.try_emplace(
        name,
        std::make_shared<Scene>(name)
    ).first->second;

    if(as_current)
    {
        _Project->SetActiveScene(name);
    }
}

void ProjectLayer::_OnSelectScene(Event::SelectScene& e)
{
    ACDA_ASSERT(_Project);

    const auto& [name] = e.data_tuple;
    _Project->SetActiveScene(name);
}

void ProjectLayer::_OnCloseScene(Event::CloseScene& e)
{
    ACDA_ASSERT(_Project);

    _Project->SetActiveScene();
}

void ProjectLayer::_OnDeleteScene(Event::DeleteScene& e)
{
    ACDA_ASSERT(_Project);
    ACDA_ASSERT(_Project->HasActiveScene());

    const auto& scene_name = _Project->GetActiveScene().Name;

    auto res = pfd::message{
        "Delete Scene",
        std::format("Do you want to delete scene: {}", scene_name)
    }.result();

    switch(res)
    {
        case pfd::button::ok:
        {
            _Project->SceneSptrStorage.erase(scene_name);
            _Project->SetActiveScene();
            break;
        }
        case pfd::button::cancel:
        default:
        {
            break;
        }
    }

}

void ProjectLayer::_OnNewEntity(Event::NewEntity& e)
{
    const auto& [type] = e.data_tuple;

    auto& scene = _AssertAndGetScene();

    std::string temp_name = "New Entity";
    std::string name = temp_name;
    int postfix{ 1 };
    while(scene.Contains(name))
    {
        name = std::format("{} {}", temp_name, ++postfix);
    }

    scene.Create(name, type);

    Match<void>(
        type,
        "actor"s,
        [&]()
    {
        scene.Emplace<ModelComponent>(name).Snapshot();

        scene.Emplace<PhysicsComponent>(name).Snapshot();

        auto& transform_comp =  scene.Emplace<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseRotation;
    },
        "camera"s,
        [&]()
    {
        scene.Emplace<CameraComponent>(name).Snapshot();

        auto& transform_comp = scene.Emplace<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseDirection;
    },
        "light"s,
        [&]()
    {
        scene.Emplace<LightComponent>(name).Snapshot();

        auto& transform_comp = scene.Emplace<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseDirection;
    }
    );
}

void ProjectLayer::_OnRenameEntity(Event::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.data_tuple;

    _Project->GetActiveScene().Rename(old_name, new_name);
}

void ProjectLayer::_OnDeleteEntity(Event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    auto& scene = _AssertAndGetScene();
    scene.Destroy(entity);
}

void ProjectLayer::_OnAddComponent(Event::AddComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _AssertAndGetScene();

    Match<void>(
        type_str,
        CameraComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<CameraComponent>(entity);
    },
        LightComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<LightComponent>(entity);
    },
        ModelComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<ModelComponent>(entity);
    },
        PhysicsComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<PhysicsComponent>(entity);
    }
    );
}

void ProjectLayer::_OnRemoveComponent(Event::RemoveComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _AssertAndGetScene();

    Match<void>(
        type_str,
        CameraComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<CameraComponent>(entity);
    },
        LightComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<LightComponent>(entity);
    },
        ModelComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<ModelComponent>(entity);
    },
        PhysicsComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<PhysicsComponent>(entity);
    }
    );
}

