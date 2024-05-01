#pragma once

#include<memory>

#include"core/base.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_layer.hpp"

#include"project/project_layer.hpp"

namespace Arcadia
{
    struct EditorContext
    {
    public:
        using self_type = EditorContext;
    public:
        static auto Instance() -> self_type&;

    public:
        std::weak_ptr<Arcadia::WindowLayer> MainWindowLayer{};
        std::weak_ptr<Arcadia::ImguiLayer> MainImguiLayer{};
        std::weak_ptr<Arcadia::ProjectLayer> MainProjectLayer{};

        bool InPlayMode{ false };
    };
}
