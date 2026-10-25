#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include "app/app.hpp"
#include "app/runtime_config.hpp"
#include "app/services.hpp"
#include "foundation/event_bus.hpp"

namespace Arcadia
{
    struct CommandList;

    struct WindowService;
    struct PhysicsService;
    struct SceneService;
    struct RendererService;
    struct ScriptService;
    struct PlayerService;
    struct ProjectService;
    struct UiService;
    struct EditorApp;

    struct FrameInfo
    {
    public:
        std::chrono::nanoseconds DeltaTime{};
        std::uint64_t FrameIndex{ 0 };

    public:
        [[nodiscard]] auto Seconds() const -> double
        {
            return std::chrono::duration<double>{ DeltaTime }.count();
        }
    };

    // Owns the state and the frame loop. The frame order is the body of Frame();
    // services are reached through the interfaces in services.hpp.
    struct Engine
    {
    public:
        RuntimeConfig config{};
        Foundation::EventBus events{};
        FrameInfo frame{};

        // Frame seams. A headless host can point every one of them at a stub.
        Platform* platform{ nullptr };
        Physics* physics{ nullptr };
        Renderer* renderer{ nullptr };
        ScriptHost* scripts{ nullptr };
        Players* players{ nullptr };
        SceneCommit* sceneCommit{ nullptr };

        // Concrete services. The frame loop goes through the seams above, the
        // services and the editor reach their peers here.
        WindowService* window{ nullptr };
        PhysicsService* physics_service{ nullptr };
        SceneService* scenes{ nullptr };
        RendererService* renderer_service{ nullptr };
        ScriptService* script_service{ nullptr };
        PlayerService* player_service{ nullptr };
        ProjectService* project{ nullptr };
        UiService* ui{ nullptr };
        EditorApp* editor{ nullptr };

        // Undo stack. It lives with the composition root, which owns it.
        CommandList* commands{ nullptr };

    public:
        void Run(App& app);
        void Frame(App& app);
        void Stop();

        [[nodiscard]] auto IsRunning() const -> bool { return _Running; }

        [[nodiscard]] auto GetStepProfile() const -> const std::unordered_map<std::string, double>&
        {
            return _StepProfile;
        }
        [[nodiscard]] auto FormatStepProfile() const -> std::string;

        // Times one step and accumulates it into the profile.
        template<class Function>
        void Step(std::string_view name, Function&& function)
        {
            const auto begin = std::chrono::steady_clock::now();
            function();
            const auto end = std::chrono::steady_clock::now();
            _StepProfile[std::string(name)] += std::chrono::duration<double, std::milli>(end - begin).count();
        }

    private:
        std::chrono::steady_clock::time_point _LastFrameTime{};
        std::unordered_map<std::string, double> _StepProfile{};
        bool _Running{ true };
    };
}
