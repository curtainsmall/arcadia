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

#include"editor/editor_context.hpp"

Arcadia::ProjectLayer::ProjectLayer():
    Arcadia::iLayer("project")
{
    const auto& app_config = Arcadia::AppConfig::Instance();
    auto& event_queue = Arcadia::EventQueue::Instance();

    Arcadia::Match<void>(
        app_config.GraphicApi,
        [&](const Arcadia::GraphicApi::Opengl&)
    {
        static Arcadia::OpenglContext gl_context{};
        _Renderer = std::make_shared<Arcadia::GlRenderer>(app_config.WorkingDirectory / Arcadia::ToFilepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
    event_queue.Signal<Arcadia::Event::RendererBuilt>(_Renderer);

    _PhysicsSimulator = std::make_shared<Arcadia::PhysicsSimulator>();
    event_queue.Signal<Arcadia::Event::PhysicsSimulatorBuilt>(_PhysicsSimulator);
}

Arcadia::ProjectLayer::~ProjectLayer()
{
    auto& event_queue = Arcadia::EventQueue::Instance();
    event_queue.Signal<Arcadia::Event::RendererUnbuilt>();
    event_queue.Signal<Arcadia::Event::PhysicsSimulatorUnbuilt>();
}

void Arcadia::ProjectLayer::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::WindowShouldClose>(ARCADIA_BIND_MEMBER_FN(_OnWindowShouldClose))
        .Dispatch<Arcadia::Event::CreateProject>(ARCADIA_BIND_MEMBER_FN(_OnCreateProject))
        .Dispatch<Arcadia::Event::OpenProject>(ARCADIA_BIND_MEMBER_FN(_OnOpenProject))
        .Dispatch<Arcadia::Event::SaveProject>(ARCADIA_BIND_MEMBER_FN(_OnSaveProject))
        .Dispatch<Arcadia::Event::SaveProjectAs>(ARCADIA_BIND_MEMBER_FN(_OnSaveProjectAs))
        .Dispatch<Arcadia::Event::CloseProject>(ARCADIA_BIND_MEMBER_FN(_OnCloseProject))
        .Dispatch<Arcadia::Event::ProjectSaved>(ARCADIA_BIND_MEMBER_FN(_OnProjectSaved))
        .Dispatch<Arcadia::Event::CreateScene>(ARCADIA_BIND_MEMBER_FN(_OnCreateScene))
        .Dispatch<Arcadia::Event::SelectScene>(ARCADIA_BIND_MEMBER_FN(_OnSelectScene))
        .Dispatch<Arcadia::Event::CloseScene>(ARCADIA_BIND_MEMBER_FN(_OnCloseScene))
        .Dispatch<Arcadia::Event::DeleteScene>(ARCADIA_BIND_MEMBER_FN(_OnDeleteScene))
        .Dispatch<Arcadia::Event::NewEntity>(ARCADIA_BIND_MEMBER_FN(_OnNewEntity))
        .Dispatch<Arcadia::Event::DeleteEntity>(ARCADIA_BIND_MEMBER_FN(_OnDeleteEntity))
        .Dispatch<Arcadia::Event::AddComponent>(ARCADIA_BIND_MEMBER_FN(_OnAddComponent))
        .Dispatch<Arcadia::Event::RemoveComponent>(ARCADIA_BIND_MEMBER_FN(_OnRemoveComponent))
        .Result();
}

void Arcadia::ProjectLayer::OnUpdate()
{}

auto Arcadia::ProjectLayer::_AssertAndGetScene() -> Arcadia::Scene&
{
    ARCADIA_ASSERT(_Project);
    ARCADIA_ASSERT(_Project->HasActiveScene());

    return _Project->GetActiveScene();
}

void Arcadia::ProjectLayer::_SaveProject()
{
    ARCADIA_ASSERT(_Project);

    auto json = _Project->ToJson();

    auto ofs = Arcadia::File::CreateOFstream(_ProjectFilepath);
    ofs << std::setw(4) << json;

    Arcadia::EventQueue::Instance().Signal<Arcadia::Event::ProjectSaved>();
}

void Arcadia::ProjectLayer::_LoadProject()
{
    ARCADIA_ASSERT(!_Project);

    auto ifs = Arcadia::File::CreateIFstream(_ProjectFilepath);
    auto json = nlohmann::json::parse(ifs);

    _Project = std::make_shared<Arcadia::Project>(json);
    Arcadia::EventQueue::Instance().Signal<Arcadia::Event::ProjectLoaded>();
}

void Arcadia::ProjectLayer::_OnWindowShouldClose(Arcadia::Event::WindowShouldClose& e)
{
    auto& event_queue = Arcadia::EventQueue::Instance();

    auto main_window_layer = Arcadia::EditorContext::Instance()._MainWindowLayer.lock();

    const auto& [p_wnd] = e.data_tuple;
    if(p_wnd == main_window_layer.get() && _Project)
    {
        auto& memento_list = Arcadia::MementoList::Instance();
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
                    event_queue.Signal<Arcadia::Event::WindowCloseCanceled>(p_wnd);
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
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::ProjectUnbuilt>();

    }
}

void Arcadia::ProjectLayer::_OnCreateProject(Arcadia::Event::CreateProject& e)
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
    _Project = std::make_shared<Arcadia::Project>(name);
    _ProjectFilepath = filepath_str.size() ? Arcadia::ToFilepath(filepath_str) : std::filesystem::path{};

    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectBuilt>(_Project);
}

void Arcadia::ProjectLayer::_OnOpenProject(Arcadia::Event::OpenProject& e)
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
        std::vector<std::string>{"Arcadia Project",std::format("*{}",Arcadia::Project::ProjectExtensionStr)}
    }.result();
    _ProjectFilepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_ProjectFilepath.empty())
    {
        return;
    }
    if(_ProjectFilepath.extension() != Arcadia::Project::ProjectExtensionStr)
    {
        pfd::message msg{
            "Open Project",
            std::format("Arcadia project must ends with extension \"{}\" while {} does not",Arcadia::Project::ProjectExtensionStr,_ProjectFilepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _LoadProject();
    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectBuilt>(_Project);
}

void Arcadia::ProjectLayer::_OnSaveProject(Arcadia::Event::SaveProject& e)
{
    ARCADIA_ASSERT(_Project);

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

void Arcadia::ProjectLayer::_OnSaveProjectAs(Arcadia::Event::SaveProjectAs& e)
{
    ARCADIA_ASSERT(_Project);

    _ProjectFilepath = pfd::save_file{
        "Save as"
    }.result();
    if(_ProjectFilepath.empty())
    {
        return;
    }
    _SaveProject();
}

void Arcadia::ProjectLayer::_OnCloseProject(Arcadia::Event::CloseProject& e)
{
    ARCADIA_ASSERT(_Project);

    auto& event_queue = Arcadia::EventQueue::Instance();

    auto& memento_list = Arcadia::MementoList::Instance();
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
    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectUnbuilt>();
}

void Arcadia::ProjectLayer::_OnProjectSaved(Arcadia::Event::ProjectSaved& e)
{
    Arcadia::MementoList::Instance().Clear();
}

void Arcadia::ProjectLayer::_OnCreateScene(Arcadia::Event::CreateScene& e)
{
    ARCADIA_ASSERT(_Project);

    const auto& [name, as_current] = e.data_tuple;
    auto& scene = _Project->SceneSptrStorage.try_emplace(
        name,
        std::make_shared<Arcadia::Scene>(name)
    ).first->second;

    if(as_current)
    {
        _Project->SetActiveScene(name);
    }
}

void Arcadia::ProjectLayer::_OnSelectScene(Arcadia::Event::SelectScene& e)
{
    ARCADIA_ASSERT(_Project);

    const auto& [name] = e.data_tuple;
    _Project->SetActiveScene(name);
}

void Arcadia::ProjectLayer::_OnCloseScene(Arcadia::Event::CloseScene& e)
{
    ARCADIA_ASSERT(_Project);

    _Project->SetActiveScene();
}

void Arcadia::ProjectLayer::_OnDeleteScene(Arcadia::Event::DeleteScene& e)
{
    ARCADIA_ASSERT(_Project);
    ARCADIA_ASSERT(_Project->HasActiveScene());

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

void Arcadia::ProjectLayer::_OnNewEntity(Arcadia::Event::NewEntity& e)
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
}

void Arcadia::ProjectLayer::_OnDeleteEntity(Arcadia::Event::DeleteEntity& e)
{
    const auto& [entity] = e.data_tuple;
    auto& scene = _AssertAndGetScene();
    scene.Destroy(entity);
}

void Arcadia::ProjectLayer::_OnAddComponent(Arcadia::Event::AddComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _AssertAndGetScene();

    Arcadia::Match<void>(
        type_str,
        Arcadia::CameraComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<Arcadia::CameraComponent>(entity);
    },
        Arcadia::LightComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<Arcadia::LightComponent>(entity);
    },
        Arcadia::ModelComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<Arcadia::ModelComponent>(entity);
    },
        Arcadia::PhysicsComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Emplace<Arcadia::PhysicsComponent>(entity);
    }
    );
}

void Arcadia::ProjectLayer::_OnRemoveComponent(Arcadia::Event::RemoveComponent& e)
{
    const auto& [entity, type_str] = e.data_tuple;
    auto& scene = _AssertAndGetScene();

    Arcadia::Match<void>(
        type_str,
        Arcadia::CameraComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<Arcadia::CameraComponent>(entity);
    },
        Arcadia::LightComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<Arcadia::LightComponent>(entity);
    },
        Arcadia::ModelComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<Arcadia::ModelComponent>(entity);
    },
        Arcadia::PhysicsComponent::GetTypeStrStatic(),
        [&]()
    {
        scene.Remove<Arcadia::PhysicsComponent>(entity);
    }
    );
}

