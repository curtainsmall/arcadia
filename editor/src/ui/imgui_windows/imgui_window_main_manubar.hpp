#pragma once

#include <memory>
#include <string>

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer.hpp"
#include "platform/api_def.hpp"
#include "ui/imgui_window.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"

namespace Arcadia
{
    class ImguiWindowPopupFunctor_CreateProject
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_CreateProject;
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        std::string _Name{};
        std::string _FilepathString{};
    };

    class ImguiWindowPopupFunctor_CreateScene
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_CreateScene;
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        std::string _Name{};
        bool _AsCurrent{ true };
        bool _NameAvailable{ true };
    };

    class ImguiWindowPopupFunctor_RenameScene
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_RenameScene;
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        std::string _PrevName{};
        std::string _NewName{};
        bool _Initailized{ false };
        bool _NameAvailable{ false };
    };

    class ImguiWindowMainMenubar: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowMainMenubar;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

        ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs):
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
        ImguiWindowPopupFunctor_CreateProject _ImguiWindowPopupFunctor_CreateProject{};
        ImguiWindowPopupFunctor_CreateScene _ImguiWindowPopupFunctor_CreateScene{};
        ImguiWindowPopupFunctor_RenameScene _ImguiWindowPopupFunctor_RenameScene{};

        std::weak_ptr<const Project> _wpProject{};

        std::vector<std::tuple<std::string, std::string>> _ImguiWindowTitleAndIdStringPairs{};

        bool _ShouldShowGizmo{ true };
    };
}