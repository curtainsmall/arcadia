#include "app/engine.hpp"

#include <format>

namespace Arcadia
{
    void Engine::Run(App& app)
    {
        app.Build(*this);

        _LastFrameTime = std::chrono::steady_clock::now();
        _Running = true;

        while(_Running)
        {
            Frame(app);
        }

        app.OnStop(*this);
    }

    void Engine::Frame(App& app)
    {
        const auto now = std::chrono::steady_clock::now();
        frame.DeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _LastFrameTime);
        _LastFrameTime = now;

        // Frame order: input, events, simulation, scene, rendering, scripts,
        // players, application hook, present.
        Step(
            "window.poll",
            [this]() -> void
            {
                if(platform != nullptr)
                {
                    platform->PollEvents();
                }
            }
        );

        Step(
            "events.dispatch",
            [this]() -> void
            {
                events.DispatchQueued();
            }
        );

        Step(
            "physics.step",
            [this]() -> void
            {
                if(physics != nullptr)
                {
                    physics->Step(frame.DeltaTime);
                    physics->ApplyToWorld();
                }
            }
        );

        Step(
            "scene.commit",
            [this]() -> void
            {
                if(sceneCommit != nullptr)
                {
                    sceneCommit->Commit();
                }
            }
        );

        Step(
            "render",
            [this]() -> void
            {
                if(renderer != nullptr)
                {
                    renderer->Draw();
                }
            }
        );

        Step(
            "scripts",
            [this]() -> void
            {
                if(scripts != nullptr)
                {
                    scripts->Update();
                }
            }
        );

        Step(
            "players",
            [this]() -> void
            {
                if(players != nullptr)
                {
                    players->Update();
                }
            }
        );

        Step(
            "app.frame",
            [this, &app]() -> void
            {
                app.OnFrame(*this);
            }
        );

        Step(
            "present",
            [this]() -> void
            {
                if(platform != nullptr)
                {
                    platform->SwapBuffers();
                }
            }
        );

        frame.FrameIndex++;
    }

    void Engine::Stop()
    {
        _Running = false;
    }

    auto Engine::FormatStepProfile() const -> std::string
    {
        std::string report{};
        // Fixed order so the report is diffable between runs.
        constexpr std::string_view order[]{
            "window.poll","events.dispatch","physics.step","scene.commit","render","scripts","players","app.frame","present"
        };

        for(const std::string_view name : order)
        {
            auto iter = _StepProfile.find(std::string(name));
            if(iter != _StepProfile.end())
            {
                report.append(std::format("{:<16} {:>9.3f} ms\n", name, iter->second));
            }
        }
        return report;
    }
}


