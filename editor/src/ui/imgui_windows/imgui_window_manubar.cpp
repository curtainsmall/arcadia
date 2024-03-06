#include "imgui_window_manubar.hpp"

#include"function/ui/imgui_header.hpp"

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
    auto has_project = _project_wptr.use_count();
    auto project_sptr = _project_wptr.lock();

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
    auto has_project = _project_wptr.use_count();
    auto project_sptr = _project_wptr.lock();

    auto& event_queue = arcadia::event_queue::instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, has_project))
        {
            event_queue.signal<arcadia::event::new_scene>();
        }

        bool has_scene = has_project && project_sptr->scene_sptr_umap.size();

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
        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, project_sptr.get()))
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
