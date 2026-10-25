#pragma once

#include <chrono>
#include <cstdint>

namespace Arcadia
{
    // Interfaces the frame loop goes through. Nothing else in the engine is
    // polymorphic, so a headless test can replace every one of them.
    struct Platform
    {
    public:
        virtual ~Platform() = default;

        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;
    };

    struct Physics
    {
    public:
        virtual ~Physics() = default;

        virtual void Step(std::chrono::nanoseconds delta_time) = 0;
        virtual void ApplyToWorld() = 0;
    };

    struct Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void Draw() = 0;
    };

    struct ScriptHost
    {
    public:
        virtual ~ScriptHost() = default;

        virtual void Update() = 0;
    };

    struct Players
    {
    public:
        virtual ~Players() = default;

        virtual void Update() = 0;
    };

    struct SceneCommit
    {
    public:
        virtual ~SceneCommit() = default;

        // Publishes the component state of the active scene to its peers.
        virtual void Commit() = 0;
    };
}
