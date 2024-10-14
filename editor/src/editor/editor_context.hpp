#pragma once

#include<memory>

#include"core/base.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_layer.hpp"

#include"project/project_layer.hpp"

struct EditorContext
{
public:
    using self_type = EditorContext;
public:
    static auto instance() -> self_type&;

public:
    std::weak_ptr<WindowLayer> main_window_layer{};
    std::weak_ptr<ImguiLayer> main_imgui_layer{};
    std::weak_ptr<ProjectLayer> main_project_layer{};

    bool in_play_mode{ false };

    float ui_scale ={ -1.0f };
};