#pragma once

#include<memory>

#include"core/base.hpp"
#include"function/ui/imgui_layer.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_layer.hpp"

namespace Arcadia
{
    struct ARCADIA_API EditorContext
    {
    public:
        using self_type = EditorContext;
    public:
        static auto Instance() -> self_type&;

    public:
        std::weak_ptr<Arcadia::WindowLayer> _MainWindowLayer{};
        std::weak_ptr<Arcadia::ImguiLayer> _MainImguiLayer{};
        std::weak_ptr<Arcadia::ProjectLayer> _MainProjectLayer{};
    };
}
