#pragma once

#include<memory>

#include"platform/api_def.hpp"
#include"function/window/window_layer.hpp"
#include"ui/imgui_layer.hpp"

#include"project/project_layer.hpp"

namespace Arcadia
{
    class EditorContext
    {
    public:
        using SelfType = EditorContext;
    public:
        static auto Instance() -> SelfType&;

    public:
        std::weak_ptr<WindowLayer> MainWindowLayer{};
        std::weak_ptr<ImguiLayer> MainImguiLayer{};
        std::weak_ptr<ProjectLayer> MainProjectLayer{};

        bool InPlayMode{ false };

        float UiScale ={ -1.0f };
    };
}