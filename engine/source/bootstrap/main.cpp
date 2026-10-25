#include <filesystem>
#include <memory>

#include "app/engine.hpp"
#include "core/command.hpp"
#include "editor/editor/editor_app.hpp"
#include "function/physics/physics_service.hpp"
#include "function/player/player_service.hpp"
#include "function/render/renderer_service.hpp"
#include "function/script/script_service.hpp"
#include "function/window/window_service.hpp"
#include "platform/graphic_api.hpp"
#include "resource/scene_service.hpp"

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

// Composition root: owns the services, wires them into the engine and hands the
// frame loop over to the editor.
ACDA_MAIN_FN_DECL
{
    Arcadia::Engine engine{};

    Arcadia::CommandList& commands = Arcadia::CommandList::Instance();
    engine.commands = &commands;

    // `.acdacfg` is read before the services exist and written back after they stopped
    const std::filesystem::path config_filepath{ Arcadia::RuntimeConfig::FileExtension };
    engine.config.Load(config_filepath);

    const Arcadia::RuntimeConfig& config = engine.config;

    // Window
    std::unique_ptr<Arcadia::WindowService> window_service = std::make_unique<Arcadia::WindowService>(
        engine,
        glm::i32vec2{ config.WindowWidth, config.WindowHeight },
        config.WindowTitle,
        config.MultisampleCount
    );
    engine.window = window_service.get();
    engine.platform = window_service.get();

    // Scene
    std::unique_ptr<Arcadia::SceneService> scene_service = std::make_unique<Arcadia::SceneService>(engine);
    engine.scenes = scene_service.get();
    engine.sceneCommit = scene_service.get();

    // Physics
    std::unique_ptr<Arcadia::PhysicsService> physics_service = std::make_unique<Arcadia::PhysicsService>(engine);
    engine.physics_service = physics_service.get();
    engine.physics = physics_service.get();

    // Renderer
    std::unique_ptr<Arcadia::RendererService> renderer_service = std::make_unique<Arcadia::RendererService>(
        engine,
        Arcadia::GraphicApi::FromConfig(config),
        config.WorkingDirectory
    );
    engine.renderer_service = renderer_service.get();
    engine.renderer = renderer_service.get();

    // Script
    std::unique_ptr<Arcadia::ScriptService> script_service = std::make_unique<Arcadia::ScriptService>(engine);
    engine.script_service = script_service.get();
    engine.scripts = script_service.get();

    // Player
    std::unique_ptr<Arcadia::PlayerService> player_service = std::make_unique<Arcadia::PlayerService>(engine);
    engine.player_service = player_service.get();
    engine.players = player_service.get();

    // Editor: builds the project and the UI services, then drives the frame
    Arcadia::EditorApp editor{ engine };
    engine.Run(editor);

    engine.config.Save(config_filepath);
    return 0;
}
