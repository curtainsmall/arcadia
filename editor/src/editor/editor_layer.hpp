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

    virtual void on_event(EventBase& e) override;
    virtual void on_update() override;
private:
    void _imgui_window_installer(ImguiLayer& imgui_layer);
    void _stop();

    void _on_window_should_close(events::WindowShouldClose& e);
    void _on_project_unbuilt(events::ProjectUnbuilt& e);
    void _on_window_close_canceled(events::WindowCloseCanceled& e);
    void _on_play_mode(events::PlayMode& e);
    void _on_input_key(events::InputKey& e);

private:
    bool _waiting_for_project_unbuilt_before_closing{ false };
};

ACDA_API auto create_application() -> std::unique_ptr<iAppLayer>;
