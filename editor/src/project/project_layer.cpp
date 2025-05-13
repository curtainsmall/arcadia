#include "project_layer.hpp"

#include "core/app/app_config.hpp"
#include "core/assert.hpp"
#include "core/command.hpp"
#include "core/enum.hpp"
#include "core/file.hpp"
#include "core/pfd.hpp"
#include "core/function.hpp"
#include "core/hash.hpp"
#include "core/log.hpp"
#include "core/match.hpp"
#include "function/render/opengl/gl_renderer.hpp"
#include "function/window/window_events.hpp"
#include "resource/components/camera_component.hpp"
#include "resource/components/light_component.hpp"
#include "resource/components/model_component.hpp"
#include "resource/components/physics_component.hpp"
#include "resource/components/transform_component.hpp"

#include "editor/editor_context.hpp"

Arcadia::ProjectLayer::ProjectLayer():
    LayerInterface("project")
{
}

Arcadia::ProjectLayer::~ProjectLayer()
{
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

        .IsDispatched();
}

void Arcadia::ProjectLayer::OnUpdate()
{
}

auto Arcadia::ProjectLayer::HasProject() const -> bool
{
    return !!_spProject;
}

void Arcadia::ProjectLayer::_SaveProject()
{
    ACDA_ASSERT(_spProject);

    nlohmann::json json = _spProject->ToJson();

    std::ofstream ofs(_ProjectFilepath);
    ofs << std::setw(4) << json;

    EventQueue::Instance().Signal<Events::ProjectSaved>();
}

void Arcadia::ProjectLayer::_LoadProject()
{
    ACDA_ASSERT(!_spProject);

    std::ifstream ifs(_ProjectFilepath);
    if(!ifs.is_open())
    {
        throw Exceptions::FileOpenFailed(std::format("Failed to open {}", _ProjectFilepath.generic_string()));
    }

    nlohmann::json json{};
    ifs >> json;
    _spProject = std::make_shared<Project>(json);

    EventQueue::Instance().Signal<Events::ProjectLoaded>();
}

void Arcadia::ProjectLayer::_OnWindowShouldClose(Events::WindowShouldClose& e)
{
    EventQueue& event_queue = EventQueue::Instance();

    std::shared_ptr<WindowLayer> main_window_layer_sptr = EditorContext::Instance().wpMainWindowLayer.lock();

    if(e.pWindowLayer == main_window_layer_sptr.get() && _spProject)
    {
        EventQueue::Instance()
            .Signal<Events::CloseProject>();
    }
}

void Arcadia::ProjectLayer::_OnCreateProject(Events::CreateProject& e)
{
    if(_spProject)
    {
        pfd::button res = pfd::message{
            "Arcadia - Unsaved Project",
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
                                        "Arcadia - Save as"
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
        _spProject.reset();
    }

    _spProject = std::make_shared<Project>(e.Name);
    _ProjectFilepath = e.FilepathString.size() ? ToFilepath(e.FilepathString) : std::filesystem::path{};

    EventQueue::Instance()
        .Signal<Events::ProjectBuilt>(_spProject);
}

void Arcadia::ProjectLayer::_OnOpenProject(Events::OpenProject& e)
{
    if(_spProject)
    {
        pfd::button res = pfd::message{
            "Arcadia - Unsaved Project",
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
                                        "Arcadia - Save as"
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
        _spProject.reset();
    }

    std::vector<std::string> filepathes = pfd::open_file{
        "Arcadia - Open",
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
        (void) pfd::message(
            "Arcadia - Open Project",
            std::format("Arcadia project must ends with extension \"{}\" while {} does not", Project::ProjectExtensionString, _ProjectFilepath.generic_string()),
            pfd::choice::ok,
            pfd::icon::info
        );
        return;
    }

    try
    {
        _LoadProject();
    }
    catch(const Exceptions::FileOpenFailed& e)
    {
        (void) pfd::notify(
            "Arcadia - Open Project",
            std::format("Failed to open project file at {}", _ProjectFilepath.generic_string()),
            pfd::icon::error
        );
        ACDA_LOG_ERROR(std::format("Failed to open project because failed to open the file at: {}", _ProjectFilepath.generic_string()));
        return;
    }
    catch(const Exceptions::ProjectConstructionFailed& e)
    {
        (void) pfd::notify(
            "Arcadia - Open Project",
            std::format("Failed to open project due to invalid project file"),
            pfd::icon::error
        );
        ACDA_LOG_ERROR(std::format("Failed to open project because its construction failed: {}", e.GetErrorMessage()));
        return;
    }

    EventQueue::Instance()
        .Signal<Events::ProjectBuilt>(_spProject);
}

void Arcadia::ProjectLayer::_OnSaveProject(Events::SaveProject& e)
{
    ACDA_ASSERT(_spProject);

    if(_ProjectFilepath.empty())
    {
        _ProjectFilepath = pfd::save_file{
            "Arcadia - Save as"
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
    ACDA_ASSERT(_spProject);

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
    ACDA_ASSERT(_spProject);

    EventQueue& event_queue = EventQueue::Instance();
    std::shared_ptr<SceneLayer> scene_layer_sptr = EditorContext::Instance().wpMainSceneLayer.lock();

    CommandList& cmd_list = CommandList::Instance();
    if(cmd_list.GetSize() || _ProjectModified || scene_layer_sptr->IsActiveSceneModified())
    {
        pfd::button res = pfd::message{
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
    EventQueue::Instance()
        .Signal<Events::ProjectUnbuilt>();
}

void Arcadia::ProjectLayer::_OnProjectSaved(Events::ProjectSaved& e)
{
    CommandList::Instance().Clear();
}
