#include "imgui_window_property.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"

void arcadia::imgui_window_property::on_event(arcadia::event_base& event)
{
    arcadia::event_dispatcher{ event }
        .dispatch<arcadia::event::open_imgui_window>(ARCADIA_BIND_MEMBER_FN(_on_open_imgui_window))
        .dispatch<arcadia::event::scene_activated>(ARCADIA_BIND_MEMBER_FN(_on_scene_activated))
        .dispatch<arcadia::event::scene_deactivated>(ARCADIA_BIND_MEMBER_FN(_on_scene_deactivated))
        .dispatch<arcadia::event::select_entity>(ARCADIA_BIND_MEMBER_FN(_on_select_entity))
        .dispatch<arcadia::event::delete_entity>(ARCADIA_BIND_MEMBER_FN(_on_delete_entity))
        .result();
}

void arcadia::imgui_window_property::on_update()
{
    if(!_open)
    {
        return;
    }

    auto has_scene = !_scene_wptr.expired();
    std::shared_ptr<const arcadia::scene> scene_sptr = _scene_wptr.lock();

    auto imgui_title = has_scene && _selected_entity != entt::null
        ? _title + " - " + scene_sptr->get_name_of_entity(_selected_entity) + get_id_str()
        : _title + get_id_str();

    auto window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(imgui_title.c_str(), &_open, window_flags))
    {
        if(!has_scene)
        {
            ImGui::Text("No property to show here");
        }
        else
        {
            _display_components();
        }
    }
    ImGui::End();
}

void arcadia::imgui_window_property::_display_components()
{
    ARCADIA_ASSERT(!_scene_wptr.expired());

    _display_camera_component();
    _display_model_component();
}

void arcadia::imgui_window_property::_display_camera_component()
{
    if(!_contains_component<arcadia::camera_component>())
    {
        return;
    }

    auto& camera_comp = _get_component<arcadia::camera_component>();
    ImGui::BeginGroup();
    ImGui::SeparatorText(camera_comp.get_type_str().c_str());
    ImGui::DragFloat3("Position", &camera_comp.pos.x);
    ImGui::DragFloat3("Target", &camera_comp.target.x);
    ImGui::DragFloat3("Up", &camera_comp.up.x);
    ImGui::DragFloat("Near Plane", &camera_comp.near_plane);
    ImGui::DragFloat("Far Plane", &camera_comp.far_plane);
    ImGui::DragFloat("FOV-Y", &camera_comp.fovy);
    ImGui::DragFloat("FOV-Y Min", &camera_comp.fovy_min);
    ImGui::DragFloat("FOV-Y Max", &camera_comp.fovy_max);
    ImGui::DragFloat("Speed", &camera_comp.speed);
    ImGui::DragInt2("Viewport Size", &camera_comp.viewport_size.x);
    ImGui::DragFloat("Sensitivity", &camera_comp.sensitivity);
    ImGui::Checkbox("Fixed Up", &camera_comp.fixed_up);
    ImGui::DragFloat("Up Epsilon", &camera_comp.up_epsilon);
    //ImGui::DragFloat2("Cursor Move Offset Range", &camera_comp.cursor_move_offset_range.x);
    ImGui::EndGroup();
}

void arcadia::imgui_window_property::_display_model_component()
{
    if(!_contains_component<arcadia::model_component>())
    {
        return;
    }

    auto& model_comp = _get_component<arcadia::model_component>();
    ImGui::BeginGroup();
    ImGui::SeparatorText(model_comp.get_type_str().c_str());
    ImGui::Text("Filepath");
    auto filepath_str = model_comp.get_filepath().empty()
        ? "(No filepath)"s
        : model_comp.get_filepath().generic_string();
    ImGui::TextWrapped(filepath_str.c_str());
    ImGui::SameLine();
    if(ImGui::Button("..."))
    {
        auto res = pfd::open_file{
            "Import Model"
        }.result();
        std::filesystem::path filepath = res.size() ? res.at(0) : "";

        if(!filepath.empty())
        {
            model_comp.import(filepath);
        }
    }
    ImGui::EndGroup();

}

void arcadia::imgui_window_property::_on_open_imgui_window(arcadia::event::open_imgui_window& e)
{
    const auto& [id_str] = e.data_tuple;
    if(id_str == get_id_str())
    {
        _open = true;
    }
}

void arcadia::imgui_window_property::_on_scene_activated(arcadia::event::scene_activated& e)
{
    const auto& [scene_wptr] = e.data_tuple;
    _scene_wptr = scene_wptr;
}

void arcadia::imgui_window_property::_on_scene_deactivated(arcadia::event::scene_deactivated& e)
{
    _scene_wptr.reset();
    _selected_entity = entt::null;
}

void arcadia::imgui_window_property::_on_select_entity(arcadia::event::select_entity& e)
{
    const auto& [entity] = e.data_tuple;
    _selected_entity = entity;
}

void arcadia::imgui_window_property::_on_delete_entity(arcadia::event::delete_entity& e)
{
    const auto& [entity] = e.data_tuple;
    if(_selected_entity == entity)
    {
        _selected_entity = entt::null;
    }
}
