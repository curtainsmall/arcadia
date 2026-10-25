#include "project.hpp"

#include "core/assert.hpp"
#include "core/command.hpp"
#include "function/physics/physics_events.hpp"
#include "function/render/renderer_events.hpp"
#include "resource/scene_events.hpp"
#include "resource/scene_service.hpp"

#include "project/project_events.hpp"

Arcadia::Project::Project(Engine& engine, std::string_view name):
    _Engine(engine),
    _Name(name)
{
}

Arcadia::Project::Project(Engine& engine, nlohmann::json& json):
    _Engine(engine),
    _Name(json.at("name"))
{
    for(const nlohmann::json& json_scene : json.at("scenes"))
    {
        _Engine.events
            .Signal<Events::CreateSceneFromJson>(json_scene);
    }
    _Engine.events
        .Signal<Events::SelectScene>(json.at("active_scene_name"), true);
}

Arcadia::Project::~Project()
{
    _Engine.events.Signal<Events::DestroyAllScenes>();
    _Engine.events.Signal<Events::RendererReset>();
    _Engine.events.Signal<Events::PhysicsSimulatorReset>();
}

auto Arcadia::Project::ToJson() const -> nlohmann::json
{
    SceneService* scene_service = _Engine.scenes;

    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",scene_service->HasActiveScene() ? scene_service->GetActiveSceneShared()->GetName() : ""}
    };

    nlohmann::json& json_scenes = json["scenes"];
    for(const auto& [name, scene_sptr] : scene_service->GetSceneStorage())
    {
        json_scenes.push_back(scene_sptr->ToJson());
    }

    return json;
}

auto Arcadia::Project::GetName() const -> std::string_view
{
    return _Name;
}

void Arcadia::Project::SetName(std::string_view name)
{
    _Name = name;
}
