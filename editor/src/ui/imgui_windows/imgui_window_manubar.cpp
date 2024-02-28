#include "imgui_window_manubar.hpp"

#include"ui/imgui_header.hpp"
#include"ui/ui_events.hpp"

void arcadia::imgui_window_menubar::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .bind_handler<arcadia::event::project_built>(ARCADIA_BIND_MEMBER_FN(_on_project_built))
        .bind_handler<arcadia::event::project_unbuilt>(ARCADIA_BIND_MEMBER_FN(_on_project_unbuilt))
        .dispatch();
}

void arcadia::imgui_window_menubar::on_update()
{
    if(ImGui::BeginMainMenuBar())
    {
        _file_menu();
        _edit_menu();

        ImGui::EndMainMenuBar();
    }
}

void arcadia::imgui_window_menubar::_file_menu()
{
    auto& event_queue = arcadia::event_queue::instance();

    if(ImGui::BeginMenu("Project"))
    {
        if(ImGui::MenuItem("New..."))
        {
            event_queue.signal<arcadia::event::new_project>();
        }
        if(ImGui::MenuItem("Open..."))
        {
            event_queue.signal<arcadia::event::open_project>();
        }
        if(ImGui::MenuItem("Save", nullptr, nullptr, _project_ptr))
        {
            event_queue.signal<arcadia::event::save_project>();
        }
        if(ImGui::MenuItem("Save As...", nullptr, nullptr, _project_ptr))
        {
            event_queue.signal<arcadia::event::save_project_as>();
        }
        if(ImGui::MenuItem("Close", nullptr, nullptr, _project_ptr))
        {
            event_queue.signal<arcadia::event::close_project>();
        }

        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_edit_menu()
{
    auto& event_queue = arcadia::event_queue::instance();
    if(ImGui::BeginMenu("Scene"))
    {
        if(ImGui::MenuItem("New...", nullptr, nullptr, _project_ptr))
        {
            event_queue.signal<arcadia::event::new_scene>();
        }

        bool has_scene{ false };
        if(_project_ptr)
        {
            has_scene = _project_ptr->scene_umap.size();
        }

        if(ImGui::BeginMenu("Select", has_scene))
        {
            for(const auto& [key, scene] : _project_ptr->scene_umap)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.signal<arcadia::event::select_scene>(key);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Delete", nullptr, nullptr, _project_ptr))
        {
            event_queue.signal<arcadia::event::delete_scene>();
        }
        ImGui::EndMenu();
    }
}

void arcadia::imgui_window_menubar::_on_project_built(arcadia::event::project_built& e)
{
    const auto& [project_ptr] = e.data_tuple;
    _project_ptr = project_ptr;
}

void arcadia::imgui_window_menubar::_on_project_unbuilt(arcadia::event::project_unbuilt& e)
{
    _project_ptr = nullptr;
}
