#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"function/ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"

namespace Arcadia
{
    struct ARCADIA_API ImguiWindowPopupCreateProject
    {
    public:
        using self_type = ImguiWindowPopupCreateProject;
    public:
        void operator()();
    public:
        bool Open{ false };
    private:
        std::string _Name{};
        std::string _FilepathStr{};
        bool _DisplayEmptyNameWarning{ true };
    };

    struct ARCADIA_API ImguiWindowPopupCreateScene
    {
    public:
        using self_type = ImguiWindowPopupCreateScene;
    public:
        void operator()(const std::shared_ptr<const Arcadia::Project>& project);
    public:
        bool Open{ false };
    private:
        std::string _Name{};
        bool _as_current{ true };
        bool _name_available{ true };
    };

    struct ARCADIA_API ImguiWindowMainMenubar: Arcadia::iImguiWindow
    {
    public:
        using self_type = ImguiWindowMainMenubar;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

        inline ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs):
            Arcadia::iImguiWindow(true, "Main Menubar"),
            _ImguiWindowTitleAndIdStrPairs(imgui_window_title_id_pairs)
        {}
        virtual ~ImguiWindowMainMenubar() = default;

        virtual void OnEvent(Arcadia::EventBase& event) override;
        virtual void OnUpdate() override;


    private:
        void _FileMenu();
        void _EditMenu();
        void _ViewMenu();

        void _OnProjectBuilt(Arcadia::Event::ProjectBuilt& e);
        void _OnProjectUnbuilt(Arcadia::Event::ProjectUnbuilt& e);

    private:
        Arcadia::ImguiWindowPopupCreateProject _ImguiWindowPopupCreateProject{};
        Arcadia::ImguiWindowPopupCreateScene _ImguiWindowPopupCreateScene{};

        std::weak_ptr<const Arcadia::Project> _Project{};

        std::vector<std::tuple<std::string, std::string>> _ImguiWindowTitleAndIdStrPairs{};
    };
}