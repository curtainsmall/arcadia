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

Arcadia::ProjectLayer::ProjectLayer() :
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
    event_queue.Signal<Events::RendererBuilt>(_Renderer);

    _PhysicsSimulator = std::make_shared<PhysicsSimulator>();
    event_queue.Signal<Events::PhysicsSimulatorBuilt>(_PhysicsSimulator);
}

Arcadia::ProjectLayer::~ProjectLayer()
{
    auto& event_queue = EventQueue::Instance();
    event_queue.Signal<Events::RendererUnbuilt>();
    event_queue.Signal<Events::PhysicsSimulatorUnbuilt>();
}

void Arcadia::ProjectLayer::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::WindowShouldClose>(ACDA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Events::CreateProject>(ACDA_BIND_MEMBER_FN(_OnCreateProject))
        .Dispatch<Events::OpenProject>(ACDA_BIND_MEMBER_FN(_OnOpenProject))
        .Dispatch<Events::SaveProject>(ACDA_BIND_MEMBER_FN(_OnSaveProject))
        .Dispatch<Events::SaveProjectAs>(ACDA_BIND_MEMBER_FN(_OnSaveProjectAs))
        .Dispatch<Events::CloseProject>(ACDA_BIND_MEMBER_FN(_OnCloseProject))
        .Dispatch<Events::ProjectSaved>(ACDA_BIND_MEMBER_FN(_OnProjectSaved))
        .Dispatch<Events::CreateScene>(ACDA_BIND_MEMBER_FN(_OnCreateScene))
        .Dispatch<Events::SelectScene>(ACDA_BIND_MEMBER_FN(_OnSelectScene))
        .Dispatch<Events::CloseScene>(ACDA_BIND_MEMBER_FN(_OnCloseScene))
        .Dispatch<Events::DeleteScene>(ACDA_BIND_MEMBER_FN(_OnDeleteScene))
        .Dispatch<Events::NewEntity>(ACDA_BIND_MEMBER_FN(_OnNewEntity))
        .Dispatch<Events::RenameEntity>(ACDA_BIND_MEMBER_FN(_OnRenameEntity))
        .Dispatch<Events::DeleteEntity>(ACDA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Events::AddComponent>(ACDA_BIND_MEMBER_FN(_OnAddComponent))
        .Dispatch<Events::RemoveComponent>(ACDA_BIND_MEMBER_FN(_OnRemoveComponent))
        .IsDispatched();
}

void Arcadia::ProjectLayer::OnUpdate()
{}

auto Arcadia::ProjectLayer::_AssertAndGetScene() -> Scene&
{
    ACDA_ASSERT(_Project);
    ACDA_ASSERT(_Project->HasActiveScene());

    return _Project->GetActiveScene();
}

void Arcadia::ProjectLayer::_SaveProject()
{
    ACDA_ASSERT(_Project);

    auto json = _Project->ToJson();

    auto ofs = File::CreateOfstream(_ProjectFilepath);
    ofs << std::setw(4) << json;

    EventQueue::Instance().Signal<Events::ProjectSaved>();
}

void Arcadia::ProjectLayer::_LoadProject()
{
    ACDA_ASSERT(!_Project);

    auto ifs = File::CreateIfstream(_ProjectFilepath);
    auto json = nlohmann::json::parse(ifs);

    _Project = std::make_shared<Project>(json);
    EventQueue::Instance().Signal<Events::ProjectLoaded>();
}

void Arcadia::ProjectLayer::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    auto& event_queue = EventQueue::Instance();

    auto main_window_layer = EditorContext::Instance().MainWindowLayer.lock();

    const auto& [p_wnd] = e.DataTuple;
    if(p_wnd == main_window_layer.get() && _Project)
    {
        auto& memento_list = MementoList::Instance();
        if(memento_list.GetSize())
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
                    event_queue.Signal<Events::WindowCloseCanceled>(p_wnd);
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
            .Signal<Events::ProjectUnbuilt>();
    }
}

void Arcadia::ProjectLayer::_OnCreateProject(Events::CreateProject& e)
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

    const auto& [name, filepath_str] = e.DataTuple;
    _Project = std::make_shared<Project>(name);
    _ProjectFilepath = filepath_str.size() ? ToFilepath(filepath_str) : std::filesystem::path{};

    EventQueue::Instance()
        .Signal<Events::ProjectBuilt>(_Project);
}

void Arcadia::ProjectLayer::_OnOpenProject(Events::OpenProject& e)
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
        std::vector<std::string>{"Arcadia Project",std::format("*{}",Project::ProjectExtensionString)}
    }.result();
    _ProjectFilepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_ProjectFilepath.empty())
    {
        return;
    }
    if(_ProjectFilepath.extension() != Project::ProjectExtensionString)
    {
        pfd::message msg{
            "Open Project",
            std::format("Arcadia project must ends with extension \"{}\" while {} does not",Project::ProjectExtensionString,_ProjectFilepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _LoadProject();
    EventQueue::Instance()
        .Signal<Events::ProjectBuilt>(_Project);
}

void Arcadia::ProjectLayer::_OnSaveProject(Events::SaveProject& e)
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

void Arcadia::ProjectLayer::_OnSaveProjectAs(Events::SaveProjectAs& e)
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

void Arcadia::ProjectLayer::_OnCloseProject(Events::CloseProject& e)
{
    ACDA_ASSERT(_Project);

    auto& event_queue = EventQueue::Instance();

    auto& memento_list = MementoList::Instance();
    if(memento_list.GetSize())
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
        .Signal<Events::ProjectUnbuilt>();
}

void Arcadia::ProjectLayer::_OnProjectSaved(Events::ProjectSaved& e)
{
    MementoList::Instance().Clear();
}

void Arcadia::ProjectLayer::_OnCreateScene(Events::CreateScene& e)
{
    ACDA_ASSERT(_Project);

    const auto& [name, as_current] = e.DataTuple;
    auto& scene = _Project->Scenes.try_emplace(
        name,
        std::make_shared<Scene>(name)
    ).first->second;

    if(as_current)
    {
        _Project->SetActiveScene(name);
    }
}

void Arcadia::ProjectLayer::_OnSelectScene(Events::SelectScene& e)
{
    ACDA_ASSERT(_Project);

    const auto& [name] = e.DataTuple;
    _Project->SetActiveScene(name);
}

void Arcadia::ProjectLayer::_OnCloseScene(Events::CloseScene& e)
{
    ACDA_ASSERT(_Project);

    _Project->SetActiveScene();
}

void Arcadia::ProjectLayer::_OnDeleteScene(Events::DeleteScene& e)
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
            _Project->Scenes.erase(scene_name);
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

void Arcadia::ProjectLayer::_OnNewEntity(Events::NewEntity& e)
{
    const auto& [type] = e.DataTuple;

    auto& scene = _AssertAndGetScene();

    std::string temp_name = "New Entity";
    std::string name = temp_name;
    int32_t postfix{ 1 };
    while(scene.ContainsEntity(name))
    {
        name = std::format("{} {}", temp_name, ++postfix);
    }

    scene.CreateEntity(name, type);

    Match<void>(
        type,
        "actor"s,
        [&]()
    {
        scene.EmplaceComponent<ModelComponent>(name).Snapshot();

        scene.EmplaceComponent<PhysicsComponent>(name).Snapshot();

        auto& transform_comp =  scene.EmplaceComponent<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseRotation;
    },
        "camera"s,
        [&]()
    {
        scene.EmplaceComponent<CameraComponent>(name).Snapshot();

        auto& transform_comp = scene.EmplaceComponent<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseDirection;
    },
        "light"s,
        [&]()
    {
        scene.EmplaceComponent<LightComponent>(name).Snapshot();

        auto& transform_comp = scene.EmplaceComponent<TransformComponent>(name);
        transform_comp.Snapshot();
        transform_comp.Flags |= TransformComponentFlags::UseDirection;
    }
    );
}

void Arcadia::ProjectLayer::_OnRenameEntity(Events::RenameEntity& e)
{
    const auto& [old_name, new_name] = e.DataTuple;

    _Project->GetActiveScene().RenameEntity(old_name, new_name);
}

void Arcadia::ProjectLayer::_OnDeleteEntity(Events::DeleteEntity& e)
{
    const auto& [entity] = e.DataTuple;
    auto& scene = _AssertAndGetScene();
    scene.DestroyEntity(entity);
}

void Arcadia::ProjectLayer::_OnAddComponent(Events::AddComponent& e)
{
    const auto& [entity, type_str] = e.DataTuple;
    auto& scene = _AssertAndGetScene();

    Match<void>(
        type_str,
        CameraComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<CameraComponent>(entity);
    },
        LightComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<LightComponent>(entity);
    },
        ModelComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<ModelComponent>(entity);
    },
        PhysicsComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.EmplaceComponent<PhysicsComponent>(entity);
    }
    );
}

void Arcadia::ProjectLayer::_OnRemoveComponent(Events::RemoveComponent& e)
{
    const auto& [entity, type_str] = e.DataTuple;
    auto& scene = _AssertAndGetScene();

    Match<void>(
        type_str,
        CameraComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<CameraComponent>(entity);
    },
        LightComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<LightComponent>(entity);
    },
        ModelComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<ModelComponent>(entity);
    },
        PhysicsComponent::GetTypeStringStatic(),
        [&]()
    {
        scene.RemoveComponent<PhysicsComponent>(entity);
    }
    );
}