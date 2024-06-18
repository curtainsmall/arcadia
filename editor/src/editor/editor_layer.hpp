#pragma once

#include<memory>

#include"core/app/app_layer.hpp"
#include"function/input/input_events.hpp"
#include"function/window/window_events.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_events.hpp"
#include"project/project_layer.hpp"
#include"ui/imgui_layer.hpp"

struct EditorAppLayer: iAppLayer
{
public:
    EditorAppLayer();
    virtual ~EditorAppLayer() = default;

    virtual void OnEvent(EventBase& event) override;
    virtual void OnUpdate() override;
private:
    void _ImguiWindowInstaller(ImguiLayer& imgui_layer);
    void _Stop();

    void _OnWindowShouldClose(Event::WindowShouldClose& e);
    void _OnProjectUnbuilt(Event::ProjectUnbuilt& e);
    void _OnWindowCloseCanceled(Event::WindowCloseCanceled& e);
    void _OnPlayMode(Event::PlayMode& e);
    void _OnInputKey(Event::InputKey& e);

private:
    bool _WaitingForProjectUnbuiltBeforeClosing{ false };
};

ARCADIA_API auto CreateApplication() -> std::unique_ptr<iAppLayer>;
