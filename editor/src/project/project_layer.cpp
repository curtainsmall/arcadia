#include "project_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/file/pfd_header.hpp"
#include"core/hash.hpp"
#include"core/log/log.hpp"
#include"core/memento/memento.hpp"
#include"function/render/opengl/gl_renderer.hpp"
#include"function/window/window_events.hpp"
#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"

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
        _spRenderer = std::make_shared<Arcadia::GlRenderer>(app_config.WorkingDirectory / Arcadia::ToFilepath("shaders/opengl"));
    },
        [](auto&&)
    {
    }
    );
    event_queue.Signal<Arcadia::Event::RendererBuilt>(_spRenderer);

    _spPhysicsSimulator = std::make_shared<Arcadia::PhysicsSimulator>();
    event_queue.Signal<Arcadia::Event::PhysicsSimulatorBuilt>(_spPhysicsSimulator);
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
    ARCADIA_ASSERT(_spProject);
    ARCADIA_ASSERT(_spProject->HasActiveScene());

    return _spProject->GetActiveScene();
}

void Arcadia::ProjectLayer::_SaveProject()
{
    ARCADIA_ASSERT(_spProject);

    auto json = _spProject->ToJson();

    auto ofs = Arcadia::File::CreateOFstream(_ProjectFilepath);
    ofs << std::setw(4) << json;

    Arcadia::EventQueue::Instance().Signal<Arcadia::Event::ProjectSaved>();
}

void Arcadia::ProjectLayer::_LoadProject()
{
    ARCADIA_ASSERT(!_spProject);

    auto ifs = Arcadia::File::CreateIFstream(_ProjectFilepath);
    auto json = nlohmann::json::parse(ifs);

    _spProject = std::make_shared<Arcadia::Project>(json);
    Arcadia::EventQueue::Instance().Signal<Arcadia::Event::ProjectLoaded>();
}

void Arcadia::ProjectLayer::_OnWindowShouldClose(Arcadia::Event::WindowShouldClose& e)
{
    auto& event_queue = Arcadia::EventQueue::Instance();

    auto main_window_layer_sptr = Arcadia::EditorContext::Instance()._wpMainWindowLayer.lock();

    const auto& [wnd_ptr] = e.data_tuple;
    if(wnd_ptr == main_window_layer_sptr.get() && _spProject)
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
                    event_queue.Signal<Arcadia::Event::WindowCloseCanceled>(wnd_ptr);
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
        _spProject.reset();
        Arcadia::EventQueue::Instance()
            .Signal<Arcadia::Event::ProjectUnbuilt>();
    }
}

void Arcadia::ProjectLayer::_OnCreateProject(Arcadia::Event::CreateProject& e)
{
    if(_spProject)
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
        _spProject.reset();
    }

    const auto& [name, filepath_str] = e.data_tuple;
    _spProject = std::make_shared<Arcadia::Project>(name);
    _ProjectFilepath = filepath_str.size() ? Arcadia::ToFilepath(filepath_str) : std::filesystem::path{};

    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectBuilt>(_spProject);
}

void Arcadia::ProjectLayer::_OnOpenProject(Arcadia::Event::OpenProject& e)
{
    if(_spProject)
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
    }

    auto filepathes = pfd::open_file{
        "Open",
        "",
        std::vector<std::string>{"Arcadia Project","*.acdaprj"}
    }.result();
    _ProjectFilepath = filepathes.size() ? filepathes.at(0) : std::string{};
    if(_ProjectFilepath.empty())
    {
        return;
    }
    if(_ProjectFilepath.extension() != ".acdaprj")
    {
        pfd::message msg{
            "Open Project",
            std::format("Arcadia project must ends with extension \".arcaprj\" while {} does not",_ProjectFilepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        };
        return;
    }
    _spProject.reset();
    _LoadProject();
    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectBuilt>(_spProject);
}

void Arcadia::ProjectLayer::_OnSaveProject(Arcadia::Event::SaveProject& e)
{
    ARCADIA_ASSERT(_spProject);

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
    ARCADIA_ASSERT(_spProject);

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
    ARCADIA_ASSERT(_spProject);

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
    _spProject.reset();
    Arcadia::EventQueue::Instance()
        .Signal<Arcadia::Event::ProjectUnbuilt>();
}

void Arcadia::ProjectLayer::_OnProjectSaved(Arcadia::Event::ProjectSaved& e)
{
    Arcadia::MementoList::Instance().Clear();
}

void Arcadia::ProjectLayer::_OnCreateScene(Arcadia::Event::CreateScene& e)
{
    ARCADIA_ASSERT(_spProject);

    const auto& [name, as_current] = e.data_tuple;
    auto& scene_sptr = _spProject->umapSceneSptr.try_emplace(
        name,
        std::make_shared<Arcadia::Scene>(name)
    ).first->second;

    if(as_current)
    {
        _spProject->SetActiveScene(name);
    }
}

void Arcadia::ProjectLayer::_OnSelectScene(Arcadia::Event::SelectScene& e)
{
    ARCADIA_ASSERT(_spProject);

    const auto& [name] = e.data_tuple;
    auto& active_scene_wptr = _spProject->SetActiveScene(name);
}

void Arcadia::ProjectLayer::_OnCloseScene(Arcadia::Event::CloseScene& e)
{
    ARCADIA_ASSERT(_spProject);

    _spProject->SetActiveScene();
}

void Arcadia::ProjectLayer::_OnDeleteScene(Arcadia::Event::DeleteScene& e)
{
    ARCADIA_ASSERT(_spProject);
    ARCADIA_ASSERT(_spProject->HasActiveScene());

    const auto& scene_name = _spProject->GetActiveScene().GetName();

    auto res = pfd::message{
        "Delete Scene",
        std::format("Do you want to delete scene: {}", scene_name)
    }.result();

    switch(res)
    {
        case pfd::button::ok:
        {
            _spProject->umapSceneSptr.erase(scene_name);
            _spProject->SetActiveScene();
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
    auto& scene = _AssertAndGetScene();

    std::string name = "New Entity";
    std::string final_name = name;
    int postfix{ 1 };
    while(scene.Contains(final_name))
    {
        final_name = std::format("{} {}", name, ++postfix);
    }

    ARCADIA_DISCARD(scene.Create(final_name));
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

