#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/physics/physics_simulator.hpp"
#include"function/render/renderer.hpp"
#include"ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"

struct ImguiWindowPopupCreateProject
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

struct ImguiWindowPopupCreateScene
{
public:
    using self_type = ImguiWindowPopupCreateScene;
public:
    void operator()(const std::shared_ptr<const Project>& project);
public:
    bool Open{ false };
private:
    std::string _Name{};
    bool _AsCurrent{ true };
    bool _NameAvailable{ true };
};

struct ImguiWindowMainMenubar: iImguiWindow
{
public:
    using self_type = ImguiWindowMainMenubar;
public:
    ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS("###menubar");

    inline ImguiWindowMainMenubar(const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs):
        iImguiWindow(true, "Main Menubar"),
        _ImguiWindowTitleAndIdStrPairs(imgui_window_title_id_pairs)
    {}
    virtual ~ImguiWindowMainMenubar() = default;

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;


private:
    void _FileMenu();
    void _EditMenu();
    void _ViewMenu();
    void _OptionMenu();

    void _OnProjectBuilt(Event::ProjectBuilt& e);
    void _OnProjectUnbuilt(Event::ProjectUnbuilt& e);

private:
    ImguiWindowPopupCreateProject _ImguiWindowPopupCreateProject{};
    ImguiWindowPopupCreateScene _ImguiWindowPopupCreateScene{};

    std::weak_ptr<const Project> _Project{};

    std::vector<std::tuple<std::string, std::string>> _ImguiWindowTitleAndIdStrPairs{};

    bool _ShowGizmo{ false };
};