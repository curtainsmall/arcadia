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
    static auto Instance() -> self_type&;

public:
    std::weak_ptr<WindowLayer> MainWindowLayer{};
    std::weak_ptr<ImguiLayer> MainImguiLayer{};
    std::weak_ptr<ProjectLayer> MainProjectLayer{};

    bool InPlayMode{ false };
};
