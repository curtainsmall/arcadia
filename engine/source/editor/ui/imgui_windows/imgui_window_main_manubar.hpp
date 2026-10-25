#pragma once

#include <memory>
#include <string>

#include "core/event.hpp"
#include "function/physics/physics_simulator.hpp"
#include "function/render/renderer.hpp"

#include "ui/imgui_window.hpp"

#include "project/project.hpp"
#include "project/project_events.hpp"

namespace Arcadia
{
    struct ImguiWindowPopupFunctor_CreateProject
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_CreateProject;
    public:
        explicit ImguiWindowPopupFunctor_CreateProject(Engine& engine):
            _Engine(engine)
        {
        }
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        Engine& _Engine;

        std::string _Name{};
        std::string _FilepathString{};
    };

    struct ImguiWindowPopupFunctor_CreateScene
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_CreateScene;
    public:
        explicit ImguiWindowPopupFunctor_CreateScene(Engine& engine):
            _Engine(engine)
        {
        }
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        Engine& _Engine;

        std::string _Name{};
        bool _AsCurrent{ true };
        bool _NameAvailable{ true };
    };

    struct ImguiWindowPopupFunctor_RenameScene
    {
    public:
        using SelfType = ImguiWindowPopupFunctor_RenameScene;
    public:
        explicit ImguiWindowPopupFunctor_RenameScene(Engine& engine):
            _Engine(engine)
        {
        }
    public:
        void operator()();
    public:
        bool Opened{ false };
    private:
        Engine& _Engine;

        std::string _PrevName{};
        std::string _NewName{};
        bool _Initailized{ false };
        bool _NameAvailable{ false };
    };

    struct ImguiWindowMainMenubar: public ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowMainMenubar;
    public:
        ACDA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

        ImguiWindowMainMenubar(
            Engine& engine,
            const std::initializer_list<std::tuple<std::string_view, std::string_view>>& imgui_window_title_id_pairs
        );
        virtual ~ImguiWindowMainMenubar() override = default;

        void OnEvent(EventBase& e);
        void OnUpdate();
    private:
        void _ShowFileMenu();
        void _ShowEditMenu();
        void _ShowViewMenu();
        void _ShowOptionMenu();

    private:
        ImguiWindowPopupFunctor_CreateProject _ImguiWindowPopupFunctor_CreateProject;
        ImguiWindowPopupFunctor_CreateScene _ImguiWindowPopupFunctor_CreateScene;
        ImguiWindowPopupFunctor_RenameScene _ImguiWindowPopupFunctor_RenameScene;

        std::vector<std::tuple<std::string_view, std::string_view>> _ImguiWindowTitleAndIdStringPairs{};

        bool _ShouldShowGizmo{ true };
    };
}