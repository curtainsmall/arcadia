#pragma once

#include<memory>

#include"core/base.hpp"
#include"function/ui/imgui_layer.hpp"
#include"function/window/window_layer.hpp"

#include"project/project_layer.hpp"

namespace arcadia
{
    struct ARCADIA_API editor_context
    {
    public:
        using self_type = editor_context;
    public:
        static auto instance() -> self_type&;

    public:
        std::weak_ptr<arcadia::window_layer> main_window_layer_wptr{};
        std::weak_ptr<arcadia::imgui_layer> main_imgui_layer_wptr{};
        std::weak_ptr<arcadia::project_layer> main_project_layer_wptr{};
    };
}
