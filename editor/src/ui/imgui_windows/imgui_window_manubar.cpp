#include "imgui_window_manubar.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"

#include"ui/ui_events.hpp"

void arcadia::imgui_window_popup_create_project::operator()()
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
            arcadia::event_queue::instance()
                .signal<arcadia::event::create_project>(
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

void arcadia::imgui_window_popup_create_scene::operator()(const std::shared_ptr<const arcadia::project>& project_sptr)
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
            _name_available = !project_sptr->scene_sptr_umap.contains(_name);
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
            arcadia::event_queue::instance()
                .signal<arcadia::event::create_scene>(
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

void arcadia::imgui_window_menubar::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::project_built>(ARCADIA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt))
        .result();
}

void arcadia::imgui_window_menubar::on_update()
{
    if(ImGui::BeginMainMenuBar())
    {
        _file_menu();
        _edit_menu();
        _view_menu();

        ImGui::EndMainMenuBar();
    }
}

void arcadia::imgui_window_menubar::_file_menu()
{
    auto has_project = !_project_wptr.expired();
    auto project_sptr = _project_wptr.lock();

    auto& event_queue = arcadia::event_queue::instance();

    _imgui_window_popup_create_project();
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            _imgui_window_popup_create_project.open = true;
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.signal<arcadia::event::open_project>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, has_project))
        {
            event_queue.signal<arcadia::event::save_project>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, has_project))
        {
            event_queue.signal<arcadia::event::save_project_as>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, has_project))
        {
            event_queue.signal<arcadia::event::close_project>();
        }

        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_edit_menu()
{
    auto has_project = !_project_wptr.expired();
    auto project_sptr = _project_wptr.lock();

    if(has_project)
    {
        _imgui_window_popup_create_scene(project_sptr);
    }
    auto& event_queue = arcadia::event_queue::instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, has_project))
        {
            _imgui_window_popup_create_scene.open = true;
        }

        bool has_scene = has_project && project_sptr->scene_sptr_umap.size();
        bool has_active_scene = has_scene && project_sptr->has_active_scene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ARCADIA_ASSERT(project_sptr.get());

            for(const auto& [key, scene] : project_sptr->scene_sptr_umap)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.signal<arcadia::event::select_scene>(key);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Close Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.signal<arcadia::event::close_scene>();
        }
        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.signal<arcadia::event::delete_scene>();
        }
        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_view_menu()
{
    auto& event_queue = arcadia::event_queue::instance();
    if(ImGui::BeginMenu("View"))
    {
        for(const auto& [title, id_str] : _imgui_window_title_and_id_str_pairs)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.signal<arcadia::event::open_imgui_window>(id_str);
                ImGui::SetWindowFocus(id_str.c_str());
            }
        }
        ImGui::EndMenu();
    }
}


void arcadia::imgui_window_menubar::_on_project_built(arcadia::event::project_built& e)
{
    const auto& [project_wptr] = e.data_tuple;
    _project_wptr = project_wptr;
}

void arcadia::imgui_window_menubar::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    _project_wptr.reset();
}



