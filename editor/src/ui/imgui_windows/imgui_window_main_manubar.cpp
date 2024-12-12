#include "imgui_window_main_manubar.hpp"

#include"core/file/pfd_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/fonts/icon_header.hpp"
#include"ui/imgui_header.hpp"

#include"ui/ui_events.hpp"

void ImguiWindowPopupCreateProject::operator()()
{
    if(!Opened)
    {
        return;
    }

    auto imgui_window_title = "Create Project"s;

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        if(ImGui::InputText("##project_name", &_Name, input_text_flags))
        {
            if(_Name.empty())
            {
                ImGui::TextColored({ 204, 80, 69, 255 }, "Project name cannot be empty");
            }
        }
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

        auto confirmed = ImGui::Button("Confirm") && !_Name.empty();
        if(confirmed)
        {
            EventQueue::Instance()
                .Signal<Events::CreateProject>(
                    _Name,
                    _FilepathString
                );
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

void ImguiWindowPopupCreateScene::operator()(const std::shared_ptr<const Project>& project)
{
    if(!Opened)
    {
        return;
    }

    auto imgui_window_title = "Create Scene"s;

    auto popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.c_str(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Opened, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_Name, input_text_flags))
        {
            _NameAvailable = !project->Scenes.contains(_Name);
            if(_Name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Scene name cannot empty");
            }
        }
        if(!_NameAvailable)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"%s\" already exsits", _Name.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::Checkbox("As current", &_AsCurrent);

        auto confirmed = ImGui::Button("Confirm") && !_Name.empty() && _NameAvailable;
        if(confirmed)
        {
            EventQueue::Instance()
                .Signal<Events::CreateScene>(
                    _Name,
                    _AsCurrent
                );
        }
        ImGui::SameLine();
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

void ImguiWindowMainMenubar::OnEvent(EventBase& e)
{
    EventDispatcher{ e }
        .Dispatch<Events::ProjectBuilt>(ACDA_BIND_MEMBER_FN(_OnProjectBuilt))
        .Dispatch<Events::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .IsDispatched();
}

void ImguiWindowMainMenubar::OnUpdate()
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

void ImguiWindowMainMenubar::_ShowFileMenu()
{
    auto project = _Project.lock();

    auto& event_queue = EventQueue::Instance();

    _ImguiWindowPopupCreateProject();
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            _ImguiWindowPopupCreateProject.Opened = true;
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.Signal<Events::OpenProject>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, !!project))
        {
            event_queue.Signal<Events::SaveProject>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, !!project))
        {
            event_queue.Signal<Events::SaveProjectAs>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, !!project))
        {
            event_queue.Signal<Events::CloseProject>();
        }

        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_ShowEditMenu()
{
    auto project = _Project.lock();

    if(project)
    {
        _ImguiWindowPopupCreateScene(project);
    }
    auto& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, !!project))
        {
            _ImguiWindowPopupCreateProject.Opened = true;
        }

        bool has_scene = project && project->Scenes.size();
        bool has_active_scene = has_scene && project->HasActiveScene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ACDA_ASSERT(project);

            for(const auto& [key, scene] : project->Scenes)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.Signal<Events::SelectScene>(key);
                }
            }
            ImGui::EndMenu();
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

void ImguiWindowMainMenubar::_ShowViewMenu()
{
    auto& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("View"))
    {
        for(const auto& [title, id_str] : _ImguiWindowTitleAndIdStringPairs)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.Signal<Events::OpenImguiWindow>(id_str);
                ImGui::SetWindowFocus(id_str.c_str());
            }
        }
        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_ShowOptionMenu()
{
    auto& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("Option"))
    {
        if(ImGui::Checkbox("Show Gizmo", &_ShouldShowGizmo))
        {
            event_queue.Signal<Events::ShowGizmo>(_ShouldShowGizmo);
        }

        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_OnProjectBuilt(Events::ProjectBuilt& e)
{
    const auto& [project] = e.DataTuple;
    _Project = project;
}

void ImguiWindowMainMenubar::_OnProjectUnbuilt(Events::ProjectUnbuilt& e)
{
    _Project.reset();
}