#pragma once

#include<memory>
#include<string>

#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"platform/api_def.hpp"
#include"ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"

namespace Arcadia
{
    class ImguiWindowPopupCreateProject
    {
    public:
        using SelfType = ImguiWindowPopupCreateProject;
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        std::string _Name{};
        std::string _FilepathString{};
        bool _ShouldShowEmptyNameWarning{ true };
    };

    class ImguiWindowPopupCreateScene
    {
    public:
        using SelfType = ImguiWindowPopupCreateScene;
    public:
        void operator()(const std::shared_ptr<const Project>& project);
    public:
        bool Opened{ false };
    private:
        std::string _Name{};
        bool _AsCurrent{ true };
        bool _NameAvailable{ true };
    };

    class ImguiWindowMainMenubar: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowMainMenubar;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

        ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs) :
            ImguiWindowInterface(true, "Main Menubar"),
            _ImguiWindowTitleAndIdStringPairs(imgui_window_title_id_pairs)
        {}
        virtual ~ImguiWindowMainMenubar() = default;

        virtual void OnEvent(EventBase& e) override;
        virtual void OnUpdate() override;
    private:
        void _ShowFileMenu();
        void _ShowEditMenu();
        void _ShowViewMenu();
        void _ShowOptionMenu();

        void _OnProjectBuilt(Events::ProjectBuilt& e);
        void _OnProjectUnbuilt(Events::ProjectUnbuilt& e);

    private:
        ImguiWindowPopupCreateProject _ImguiWindowPopupCreateProject{};
        ImguiWindowPopupCreateScene _ImguiWindowPopupCreateScene{};

        std::weak_ptr<const Project> _Project{};

        std::vector<std::tuple<std::string, std::string>> _ImguiWindowTitleAndIdStringPairs{};

        bool _ShouldShowGizmo{ false };
    };
}