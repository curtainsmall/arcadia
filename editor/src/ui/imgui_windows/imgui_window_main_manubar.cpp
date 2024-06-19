#include "imgui_window_main_manubar.hpp"

#include"core/file/pfd_header.hpp"
#include"ui/imgui_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/fonts/icon_header.hpp"

#include"ui/ui_events.hpp"

void ImguiWindowPopupCreateProject::operator()()
{
    if(!open)
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
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        if(ImGui::InputText("##project_name", &_name, input_text_flags))
        {
            if(_name.empty())
            {
                ImGui::TextColored({ 204, 80, 69, 255 }, "Project name cannot be empty");
            }
        }
        if(ImGui::Button("Project location"))
        {
            _filepath_str = pfd::save_file{
                "Select location for Project",
                std::format("{}.acdaprj",_name),
                {"Arcadia Project",".acdaprj"}
            }.result();
        }
        if(!_filepath_str.empty())
        {
            ImGui::TextColored({ 228, 228, 228, 255 }, "Location Selected: %s", _filepath_str.c_str());
        }

        auto confirmed = ImGui::Button("Confirm") && !_name.empty();
        if(confirmed)
        {
            EventQueue::instance()
                .signal<event::CreateProject>(
                    _name,
                    _filepath_str
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            open = false;
            _name.clear();
            _filepath_str.clear();
        }

        ImGui::EndPopup();
    }
}

void ImguiWindowPopupCreateScene::operator()(const std::shared_ptr<const Project>& project)
{
    if(!open)
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
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_name, input_text_flags))
        {
            _name_available = !project->scene_sptr_storage.contains(_name);
            if(_name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Scene name cannot empty");
            }
        }
        if(!_name_available)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"%s\" already exsits", _name.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::Checkbox("As current", &_as_current);

        auto confirmed = ImGui::Button("Confirm") && !_name.empty() && _name_available;
        if(confirmed)
        {
            EventQueue::instance()
                .signal<event::CreateScene>(
                    _name,
                    _as_current
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            open = false;
            _name.clear();
            _as_current = true;
            _name_available = true;
        }

        ImGui::EndPopup();
    }
}

void ImguiWindowMainMenubar::on_event(EventBase& e)
{
    EventDispatcher{ e }
        .dispatch<event::ProjectBuilt>(ACDA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<event::ProjectUnbuilt>(ACDA_BIND_MEMBER_FN(_on_project_unbuilt))
        .result();
}

void ImguiWindowMainMenubar::on_update()
{
    if(ImGui::BeginMainMenuBar())
    {
        _file_menu();
        _edit_menu();
        _view_menu();
        _option_menu();

        ImGui::EndMainMenuBar();
    }
}

void ImguiWindowMainMenubar::_file_menu()
{
    auto project = _project.lock();

    auto& event_queue = EventQueue::instance();

    _imgui_window_popup_create_project();
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            _imgui_window_popup_create_project.open = true;
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.signal<event::OpenProject>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, !!project))
        {
            event_queue.signal<event::SaveProject>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, !!project))
        {
            event_queue.signal<event::SaveProjectAs>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, !!project))
        {
            event_queue.signal<event::CloseProject>();
        }

        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_edit_menu()
{
    auto project = _project.lock();

    if(project)
    {
        _imgui_window_popup_create_scene(project);
    }
    auto& event_queue = EventQueue::instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, !!project))
        {
            _imgui_window_popup_create_scene.open = true;
        }

        bool has_scene = project && project->scene_sptr_storage.size();
        bool has_active_scene = has_scene && project->has_active_scene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ACDA_ASSERT(project);

            for(const auto& [key, scene] : project->scene_sptr_storage)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.signal<event::SelectScene>(key);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Close Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.signal<event::CloseScene>();
        }
        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.signal<event::DeleteScene>();
        }

        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_view_menu()
{
    auto& event_queue = EventQueue::instance();
    if(ImGui::BeginMenu("View"))
    {
        for(const auto& [title, id_str] : _imgui_window_title_and_id_str_pairs)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.signal<event::OpenImguiWindow>(id_str);
                ImGui::SetWindowFocus(id_str.c_str());
            }
        }
        ImGui::EndMenu();
    }
}

void ImguiWindowMainMenubar::_option_menu()
{
    auto& event_queue = EventQueue::instance();
    if(ImGui::BeginMenu("Option"))
    {
        if(ImGui::Checkbox("Show Gizmo", &_show_gizmo))
        {
            event_queue.signal<event::ShowGizmo>(_show_gizmo);
        }

        ImGui::EndMenu();
    }
}


void ImguiWindowMainMenubar::_on_project_built(event::ProjectBuilt& e)
{
    const auto& [project] = e.data_tuple;
    _project = project;
}

void ImguiWindowMainMenubar::_on_project_unbuilt(event::ProjectUnbuilt& e)
{
    _project.reset();
}



