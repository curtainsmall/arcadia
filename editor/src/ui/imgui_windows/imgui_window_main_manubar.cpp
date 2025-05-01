#include "imgui_window_main_manubar.hpp"

#include "core/assert.hpp"
#include "core/pfd.hpp"
#include "core/function.hpp"
#include "platform/graphic_api.hpp"
#include "resource/fonts/icon.hpp"
#include "ui/imgui.hpp"

#include "ui/ui_events.hpp"

void Arcadia::ImguiWindowPopupFunctor_CreateProject::operator()()
{
    if(!Opened)
    {
        return;
    }

    std::string imgui_window_title("Create Project");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        (void) ImGui::InputText("##project_name", &_Name, input_text_flags);

        if(ImGui::Button("Project location"))
        {
            _FilepathString = pfd::save_file{
                "Select location for Project",
                std::format("{}.acdaprj",_Name),
                {"Arcadia Project",".acdaprj"}
            }.result();
        }
        if(!_FilepathString.empty())
        {
            ImGui::TextColored({ 228, 228, 228, 255 }, "Location Selected: %s", _FilepathString.c_str());
        }

        bool confirmed = false;
        if(_Name.empty() || _FilepathString.empty())
        {
            ImGui::BeginDisabled();
            ImGui::Button("Confirm");
            ImGui::EndDisabled();
        }
        else
        {
            confirmed = ImGui::Button("Confirm");
            if(confirmed)
            {
                EventQueue::Instance()
                    .Signal<Events::CreateProject>(
                        _Name,
                        _FilepathString
                    );
            }
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Opened = false;
            _Name.clear();
            _FilepathString.clear();
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPopupFunctor_CreateScene::operator()(const std::shared_ptr<const Project>& project_sptr)
{
    if(!Opened)
    {
        return;
    }

    std::string imgui_window_title("Create Scene");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_Name, input_text_flags))
        {
            _NameAvailable = !project_sptr->HasScene(_Name);
        }
        if(!_NameAvailable)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"%s\" already exsits", _Name.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::Checkbox("As current", &_AsCurrent);

        bool confirmed = false;
        // When the name is invalid (empty or duplicate)
        if(_Name.empty() || !_NameAvailable)
        {
            ImGui::BeginDisabled();
            ImGui::Button("Confirm");
            ImGui::EndDisabled();
        }
        else
        {
            confirmed = ImGui::Button("Confirm");
            if(confirmed)
            {
                EventQueue::Instance()
                    .Signal<Events::CreateScene>(
                        _Name,
                        _AsCurrent
                    );
            }
        }
        ImGui::SameLine();
        // Close popup
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Opened = false;
            _Name.clear();
            _AsCurrent = true;
            _NameAvailable = true;
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPopupFunctor_RenameScene::operator()(const std::shared_ptr<const Project>& project_sptr)
{
    if(!Opened)
    {
        return;
    }

    if(!_Initailized)
    {
        _PrevName = project_sptr->GetActiveScene().GetName();
        _NewName = _PrevName;
        _Initailized = true;
    }

    std::string imgui_window_title("Rename Scene");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("New name");
        if(ImGui::InputText("##new_scene_name", &_NewName, input_text_flags))
        {
            _NameAvailable = !project_sptr->HasScene(_NewName);
        }

        bool confirmed = false;
        if(_NewName.empty() || _NewName == _PrevName || !_NameAvailable)
        {
            ImGui::BeginDisabled();
            (void) ImGui::Button("Confirm");
            ImGui::EndDisabled();
        }
        else
        {
            confirmed = ImGui::Button("Confirm");
            if(confirmed)
            {
                EventQueue::Instance()
                    .Signal<Events::RenameScene>(
                        _NewName
                    );
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel") || confirmed)
        {
            ImGui::CloseCurrentPopup();
            Opened = false;
            _NewName.clear();
            _PrevName.clear();
            _Initailized = false;
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowMainMenubar::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::ProjectBuilt>(ACDA_BIND_MEMBER_FN(_OnProjectBuilt))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .IsDispatched();
}

void Arcadia::ImguiWindowMainMenubar::OnUpdate()
{
    if(ImGui::BeginMainMenuBar())
    {
        _ShowFileMenu();
        _ShowEditMenu();
        _ShowViewMenu();
        _ShowOptionMenu();

        ImGui::EndMainMenuBar();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ShowFileMenu()
{
    std::shared_ptr<const Project> project_sptr = _wpProject.lock();

    EventQueue& event_queue = EventQueue::Instance();

    _ImguiWindowPopupFunctor_CreateProject();
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            _ImguiWindowPopupFunctor_CreateProject.Opened = true;
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.Signal<Events::OpenProject>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, !!project_sptr))
        {
            event_queue.Signal<Events::SaveProject>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, !!project_sptr))
        {
            event_queue.Signal<Events::SaveProjectAs>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, !!project_sptr))
        {
            event_queue.Signal<Events::CloseProject>();
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ShowEditMenu()
{
    std::shared_ptr<const Project> project_uptr = _wpProject.lock();

    if(project_uptr)
    {
        _ImguiWindowPopupFunctor_CreateScene(project_uptr);
        _ImguiWindowPopupFunctor_RenameScene(project_uptr);
    }
    EventQueue& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, !!project_uptr))
        {
            _ImguiWindowPopupFunctor_CreateScene.Opened = true;
        }

        bool has_scene = project_uptr && project_uptr->HasScene();
        bool has_active_scene = has_scene && project_uptr->HasActiveScene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ACDA_ASSERT(project_uptr);

            for(const auto& [key, scene] : project_uptr->GetSceneStorage())
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.Signal<Events::SelectScene>(key);
                }
            }
            ImGui::EndMenu();
        }

        if(ImGui::MenuItem("Rename Scene ...", nullptr, nullptr, has_active_scene))
        {
            _ImguiWindowPopupFunctor_RenameScene.Opened = true;
        }

        if(ImGui::MenuItem("Close Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.Signal<Events::CloseScene>();
        }

        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.Signal<Events::DeleteScene>();
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ShowViewMenu()
{
    EventQueue& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("View"))
    {
        for(const auto& [title, id_string] : _ImguiWindowTitleAndIdStringPairs)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.Signal<Events::OpenImguiWindow>(id_string);
                ImGui::SetWindowFocus(id_string.c_str());
            }
        }
        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ShowOptionMenu()
{
    EventQueue& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("Option"))
    {
        if(ImGui::Checkbox("Show Gizmo", &_ShouldShowGizmo))
        {
            event_queue.Signal<Events::ShowGizmo>(_ShouldShowGizmo);
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_OnProjectBuilt(Events::ProjectBuilt& e)
{
    _wpProject = e.spProject;
}

void Arcadia::ImguiWindowMainMenubar::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    _wpProject.reset();
}