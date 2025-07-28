#include "imgui_window_main_manubar.hpp"

#include "core/assert.hpp"
#include "core/function.hpp"
#include "core/pfd.hpp"
#include "platform/graphic_api.hpp"
#include "resource/fonts/icon.hpp"
#include "resource/scene_layer.hpp"

#include "project/project_layer.hpp"
#include "ui/imgui.hpp"
#include "ui/ui_events.hpp"

void Arcadia::ImguiWindowPopupFunctor_CreateProject::operator()()
{
    if(!Opened)
    {
        return;
    }

    EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(true);

    std::string_view imgui_window_title("Create Project");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.data(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.data(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Project name");
        (void) ImGui::InputText("##project_name", &_Name, input_text_flags);

        if(ImGui::Button("Project location"))
        {
            _FilepathString = pfd::save_file{
                "Select location for Project",
                _Name + Project::ProjectFileExtension,
                {"Arcadia Project",std::format("*{}", Project::ProjectFileExtension)}
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
            EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(false);
            _Name.clear();
            _FilepathString.clear();
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPopupFunctor_CreateScene::operator()()
{
    if(!Opened)
    {
        return;
    }

    EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(true);

    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();

    std::string_view imgui_window_title("Create Scene");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.data(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.data(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_Name, input_text_flags))
        {
            _NameAvailable = !scene_layer_sptr->HasScene(_Name);
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
            EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(false);
            _Name.clear();
            _AsCurrent = true;
            _NameAvailable = true;
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPopupFunctor_RenameScene::operator()()
{
    if(!Opened)
    {
        return;
    }

    EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(true);
    std::shared_ptr<SceneLayer> scene_layer = LayerStack::Instance().GetLayerShared<SceneLayer>();

    if(!_Initailized)
    {
        _PrevName = scene_layer->GetActiveSceneShared()->GetName();
        _NewName = _PrevName;
        _Initailized = true;
    }

    std::string_view imgui_window_title("Rename Scene");

    ImGuiPopupFlags popup_flags =
        ImGuiPopupFlags_NoOpenOverExistingPopup;
    ImGui::OpenPopup(imgui_window_title.data(), popup_flags);

    ImGui::SetNextWindowSize({ 430,120 }, ImGuiCond_Once);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoCollapse;
    if(ImGui::BeginPopupModal(imgui_window_title.data(), &Opened, window_flags))
    {
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("New name");
        if(ImGui::InputText("##new_scene_name", &_NewName, input_text_flags))
        {
            _NameAvailable = !scene_layer->HasScene(_NewName);
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
            EventQueue::Instance().Signal<Events::KeyboardInputOccupied>(false);
            _NewName.clear();
            _PrevName.clear();
            _Initailized = false;
        }

        ImGui::EndPopup();
    }
}

Arcadia::ImguiWindowMainMenubar::ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string_view, std::string_view>>& imgui_window_title_id_pairs):
    ImguiWindowInterface(true, "Main Menubar"),
    _ImguiWindowTitleAndIdStringPairs(imgui_window_title_id_pairs)
{
}

void Arcadia::ImguiWindowMainMenubar::OnEvent(EventBase& e)
{
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
    std::shared_ptr<ProjectLayer> project_layer_sptr = LayerStack::Instance().GetLayerShared<ProjectLayer>();

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
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, project_layer_sptr->HasProject()))
        {
            event_queue.Signal<Events::SaveProject>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, project_layer_sptr->HasProject()))
        {
            event_queue.Signal<Events::SaveProjectAs>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, project_layer_sptr->HasProject()))
        {
            event_queue.Signal<Events::CloseProject>();
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ShowEditMenu()
{
    auto [scene_layer_sptr, project_layer_sptr] = LayerStack::Instance().GetMultipleLayersShared<SceneLayer, ProjectLayer>();

    if(project_layer_sptr->HasProject())
    {
        _ImguiWindowPopupFunctor_CreateScene();
        _ImguiWindowPopupFunctor_RenameScene();
    }
    EventQueue& event_queue = EventQueue::Instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, project_layer_sptr->HasProject()))
        {
            _ImguiWindowPopupFunctor_CreateScene.Opened = true;
        }

        bool has_scene = project_layer_sptr->HasProject() && scene_layer_sptr->HasScene();
        bool has_active_scene = has_scene && scene_layer_sptr->HasActiveScene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ACDA_ASSERT(project_layer_sptr->HasProject());

            for(const auto& [key, scene] : scene_layer_sptr->GetSceneStorage())
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
            if(ImGui::MenuItem(title.data()))
            {
                event_queue.Signal<Events::OpenImguiWindow>(id_string);
                ImGui::SetWindowFocus(id_string.data());
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