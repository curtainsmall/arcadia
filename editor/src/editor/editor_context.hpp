#pragma once

#include <memory>

#include "platform/api_def.hpp"
#include "function/window/window_layer.hpp"
#include "function/render/renderer_layer.hpp"
#include "function/physics/physics_layer.hpp"
#include "resource/scene_layer.hpp"
#include "ui/imgui_layer.hpp"

#include "project/project_layer.hpp"

namespace Arcadia
{
    class EditorContext
    {
    public:
        using SelfType = EditorContext;
    public:
        static auto Instance() -> SelfType&;

    public:
        std::weak_ptr<WindowLayer> wpMainWindowLayer{};
        std::weak_ptr<RendererLayer> wpMainRendererLayer{};
        std::weak_ptr<PhysicsLayer> wpMainPhysicsLayer{};
        std::weak_ptr<ImguiLayer> wpMainImguiLayer{};
        std::weak_ptr<SceneLayer> wpMainSceneLayer{};
        std::weak_ptr<ProjectLayer> wpMainProjectLayer{};

        bool InPlayMode{ false };

        float UiScale ={ -1.0f };
    };
}