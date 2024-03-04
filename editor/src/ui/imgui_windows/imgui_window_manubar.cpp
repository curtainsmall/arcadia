#include "imgui_window_manubar.hpp"

#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void arcadia::imgui_window_menubar::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::project_built>(ARCADIA_BIND_MEMBER_FN(_on_project_built))
        .dispatch<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt));
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
    auto& event_queue = arcadia::event_queue::instance();

    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            event_queue.signal<arcadia::event::new_project>();
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.signal<arcadia::event::open_project>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, _project_cptr))
        {
            event_queue.signal<arcadia::event::save_project>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, _project_cptr))
        {
            event_queue.signal<arcadia::event::save_project_as>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, _project_cptr))
        {
            event_queue.signal<arcadia::event::close_project>();
        }

        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_edit_menu()
{
    auto& event_queue = arcadia::event_queue::instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, _project_cptr))
        {
            event_queue.signal<arcadia::event::new_scene>();
        }

        bool has_scene{ false };
        if(_project_cptr)
        {
            has_scene = _project_cptr->scene_umap.size();
        }

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ARCADIA_ASSERT(_project_cptr);

            for(const auto& [key, scene] : _project_cptr->scene_umap)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.signal<arcadia::event::select_scene>(key);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, _project_cptr))
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
        for(const auto& title : _imgui_window_titles)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.signal<arcadia::event::open_imgui_window>(title);
                ImGui::SetWindowFocus(title.c_str());
            }
        }
        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_on_project_built(arcadia::event::project_built& e)
{
    const auto& [project_ptr] = e.data_tuple;
    _project_cptr = project_ptr;
}

void arcadia::imgui_window_menubar::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    _project_cptr = nullptr;
}
