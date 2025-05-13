#include "project.hpp"

#include "core/assert.hpp"
#include "core/command.hpp"
#include "resource/scene_layer.hpp"

#include "project/project_events.hpp"

Arcadia::Project::Project(std::string name):
    _Name(name)
{
}

Arcadia::Project::Project(nlohmann::json& json):
    _Name(json.at("name"))
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();

    for(const nlohmann::json& json_scene : json.at("scenes"))
    {
        scene_layer_sptr->CreateScene(json_scene);
    }
    scene_layer_sptr->SetActiveScene(json.at("active_scene_name"));
}

Arcadia::Project::~Project()
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();
    scene_layer_sptr->DestroyAllScenes();
}

auto Arcadia::Project::ToJson() const -> nlohmann::json
{
    std::shared_ptr<SceneLayer> scene_layer_sptr = LayerStack::Instance().GetLayerShared<SceneLayer>();

    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",scene_layer_sptr->HasActiveScene() ? scene_layer_sptr->GetActiveSceneShared()->GetName() : ""}
    };

    nlohmann::json& json_scenes = json["scenes"];
    for(const auto& [name, scene_sptr] : scene_layer_sptr->GetSceneStorage())
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