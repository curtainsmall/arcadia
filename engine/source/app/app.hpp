#pragma once

namespace Arcadia
{
    struct Engine;

    // Editor and game implement this; nothing else in the engine is polymorphic.
    struct App
    {
    public:
        virtual ~App() = default;

        // Called once before the first frame.
        virtual void Build(Engine& engine) { (void) engine; }

        // Called once per frame, between the simulation and the present step.
        virtual void OnFrame(Engine& engine) { (void) engine; }

        // Called once after the loop stopped.
        virtual void OnStop(Engine& engine) { (void) engine; }
    };
}
