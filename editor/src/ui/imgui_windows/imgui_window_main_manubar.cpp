#include "imgui_window_main_manubar.hpp"

#include"core/file/pfd_header.hpp"
#include"function/ui/imgui_header.hpp"
#include"platform/graphic_api/graphic_api.hpp"
#include"resource/fonts/icon_header.hpp"

#include"ui/ui_events.hpp"

void Arcadia::ImguiWindowPopupCreateProject::operator()()
{
    if(!Open)
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
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Open, window_flags))
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
            _FilepathStr = pfd::save_file{
                "Select location for Project",
                std::format("{}.acdaprj",_Name),
                {"Arcadia Project",".acdaprj"}
            }.result();
        }
        if(!_FilepathStr.empty())
        {
            ImGui::TextColored({ 228, 228, 228, 255 }, "Location Selected: %s", _FilepathStr.c_str());
        }

        auto confirmed = ImGui::Button("Confirm") && !_Name.empty();
        if(confirmed)
        {
            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::CreateProject>(
                    _Name,
                    _FilepathStr
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Open = false;
            _Name.clear();
            _FilepathStr.clear();
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowPopupCreateScene::operator()(const std::shared_ptr<const Arcadia::Project>& project_sptr)
{
    if(!Open)
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
    if(ImGui::BeginPopupModal(imgui_window_title.c_str(), &Open, window_flags))
    {
        auto input_text_flags =
            ImGuiInputTextFlags_AutoSelectAll;
        ImGui::Text("Scene name");
        if(ImGui::InputText("##scene_name", &_Name, input_text_flags))
        {
            _name_available = !project_sptr->umapSceneSptr.contains(_Name);
            if(_Name.empty())
            {
                ImGui::TextColored({ 204,80,69,255 }, "Scene name cannot empty");
            }
        }
        if(!_name_available)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 204, 80, 69, 255 });
            ImGui::Text("Scene named \"%s\" already exsits", _Name.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::Checkbox("As current", &_as_current);

        auto confirmed = ImGui::Button("Confirm") && !_Name.empty() && _name_available;
        if(confirmed)
        {
            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::CreateScene>(
                    _Name,
                    _as_current
                );
        }
        ImGui::SameLine();
        if(confirmed || ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            Open = false;
            _Name.clear();
            _as_current = true;
            _name_available = true;
        }

        ImGui::EndPopup();
    }
}

void Arcadia::ImguiWindowMainMenubar::OnEvent(Arcadia::EventBase& event)
{
    Arcadia::EventDispatcher{ event }
        .Dispatch<Arcadia::Event::ProjectBuilt>(ARCADIA_BIND_MEMBER_FN(_OnProjectBuilt))
        .Dispatch<Arcadia::Event::ProjectUnbuilt>(ARCADIA_BIND_MEMBER_FN(_OnProjectUnbuilt))
        .Result();
}

void Arcadia::ImguiWindowMainMenubar::OnUpdate()
{
    if(ImGui::BeginMainMenuBar())
    {
        _FileMenu();
        _EditMenu();
        _ViewMenu();

        ImGui::EndMainMenuBar();
    }
}

void Arcadia::ImguiWindowMainMenubar::_FileMenu()
{
    auto& event_queue = Arcadia::EventQueue::Instance();

    _ImguiWindowPopupCreateProject();
    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("New Project..."))
        {
            _ImguiWindowPopupCreateProject.Open = true;
        }
        if(ImGui::MenuItem("Open Project..."))
        {
            event_queue.Signal<Arcadia::Event::OpenProject>();
        }
        if(ImGui::MenuItem("Save Project", nullptr, nullptr, !!_spProject))
        {
            event_queue.Signal<Arcadia::Event::SaveProject>();
        }
        if(ImGui::MenuItem("Save Project As...", nullptr, nullptr, !!_spProject))
        {
            event_queue.Signal<Arcadia::Event::SaveProjectAs>();
        }
        if(ImGui::MenuItem("Close Project", nullptr, nullptr, !!_spProject))
        {
            event_queue.Signal<Arcadia::Event::CloseProject>();
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_EditMenu()
{
    if(_spProject)
    {
        _ImguiWindowPopupCreateScene(_spProject);
    }
    auto& event_queue = Arcadia::EventQueue::Instance();
    if(ImGui::BeginMenu("Edit"))
    {
        if(ImGui::MenuItem("New Scene ...", nullptr, nullptr, !!_spProject))
        {
            _ImguiWindowPopupCreateScene.Open = true;
        }

        bool has_scene = _spProject && _spProject->umapSceneSptr.size();
        bool has_active_scene = has_scene && _spProject->HasActiveScene();

        if(ImGui::BeginMenu("Select Scene", has_scene))
        {
            ARCADIA_ASSERT(_spProject.get());

            for(const auto& [key, scene] : _spProject->umapSceneSptr)
            {
                if(ImGui::MenuItem(key.c_str()))
                {
                    event_queue.Signal<Arcadia::Event::SelectScene>(key);
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::MenuItem("Close Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.Signal<Arcadia::Event::CloseScene>();
        }
        if(ImGui::MenuItem("Delete Scene", nullptr, nullptr, has_active_scene))
        {
            event_queue.Signal<Arcadia::Event::DeleteScene>();
        }

        ImGui::EndMenu();
    }
}

void Arcadia::ImguiWindowMainMenubar::_ViewMenu()
{
    auto& event_queue = Arcadia::EventQueue::Instance();
    if(ImGui::BeginMenu("View"))
    {
        for(const auto& [title, id_str] : _ImguiWindowTitleAndIdStrPairs)
        {
            if(ImGui::MenuItem(title.c_str()))
            {
                event_queue.Signal<Arcadia::Event::OpenImguiWindow>(id_str);
                ImGui::SetWindowFocus(id_str.c_str());
            }
        }
        ImGui::EndMenu();
    }
}


void Arcadia::ImguiWindowMainMenubar::_OnProjectBuilt(Arcadia::Event::ProjectBuilt& e)
{
    const auto& [project_wptr] = e.data_tuple;
    _spProject = project_wptr;
}

void Arcadia::ImguiWindowMainMenubar::_OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e)
{
    _spProject.reset();
}



