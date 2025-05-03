#include "project.hpp"

#include "core/assert.hpp"
#include "core/command.hpp"

#include "project/project_events.hpp"
#include "editor/editor_context.hpp"

Arcadia::Project::Project(std::string name):
    _Name(name)
{}

Arcadia::Project::Project(nlohmann::json& json):
    _Name(json.at("name"))
{
    SceneLayer& scene_layer = *EditorContext::Instance().wpMainSceneLayer.lock();

    for(const nlohmann::json& json_scene : json.at("scenes"))
    {
        scene_layer.CreateScene(json_scene);
    }
    scene_layer.SetActiveScene(json.at("active_scene_name"));
}

Arcadia::Project::~Project()
{
    SceneLayer& scene_layer = *EditorContext::Instance().wpMainSceneLayer.lock();
    scene_layer.DestroyAllScenes();
}

auto Arcadia::Project::ToJson() const -> nlohmann::json
{
    SceneLayer& scene_layer = *EditorContext::Instance().wpMainSceneLayer.lock();

    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",scene_layer.HasActiveScene() ? scene_layer.GetActiveSceneShared()->GetName() : ""}
    };

    nlohmann::json& json_scenes = json["scenes"];
    for(const auto& [name, scene_sptr] : scene_layer.GetSceneStorage())
    {
        json_scenes.push_back(scene_sptr->ToJson());
    }

    return json;
}

auto Arcadia::Project::GetName() const -> const std::string&
{
    return _Name;
}

void Arcadia::Project::SetName(const std::string& name)
{
    _Name = name;
}
