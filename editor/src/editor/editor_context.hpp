#pragma once

#include <memory>

#include "function/physics/physics_layer.hpp"
#include "function/render/renderer_layer.hpp"
#include "function/window/window_layer.hpp"
#include "platform/api_def.hpp"
#include "resource/scene_layer.hpp"
#include "ui/imgui_layer.hpp"

#include "project/project_layer.hpp"

namespace Arcadia
{
    struct EditorContext
    {
    public:
        using SelfType = EditorContext;
    public:
        static auto Instance() -> SelfType&;

    public:
        bool InPlayMode{ false };

        float UiScale = { -1.0f };
    };
}