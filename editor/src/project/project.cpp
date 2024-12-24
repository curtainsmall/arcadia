#include "project.hpp"

#include"core/assert.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Arcadia::Project::Project(nlohmann::json& json) :
    _Name(json.at("name"))
{
    for(const nlohmann::json& json_scene : json.at("scenes"))
    {
        SceneStorage.try_emplace(json_scene.at("name"), std::make_shared<Scene>(json_scene));
    }

    SetActiveScene(json.at("active_scene_name"));
}

auto Arcadia::Project::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",HasActiveScene() ? GetActiveScene().GetName() : ""}
    };

    for(const auto& [name, scene_sptr] : SceneStorage)
    {
        json.at("scenes")
            .push_back(scene_sptr->ToJson());
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

auto Arcadia::Project::HasActiveScene() const -> bool
{
    return !!_ActiveScene;
}

auto Arcadia::Project::GetActiveScene() -> Scene&
{
    ACDA_ASSERT(HasActiveScene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_ActiveScene;
}

auto Arcadia::Project::GetActiveScene() const -> const Scene&
{
    ACDA_ASSERT(HasActiveScene());
    return *_ActiveScene;
}

void Arcadia::Project::SetActiveScene(const std::string& name)
{
    auto is_same_scene = _ActiveScene && name == _ActiveScene->GetName();

    if(!is_same_scene)
    {
        MementoList::Instance().Clear();

        if(_ActiveScene)
        {
            _ActiveScene.reset();
            EventQueue::Instance()
                .Signal<Events::SceneDeactivated>();
        }

        if(!name.empty() && SceneStorage.find(name) != SceneStorage.end())
        {
            _ActiveScene = SceneStorage.at(name);

            // Snapshot the scene but not put it into memento list
            _SnapshotEntities();

            EventQueue::Instance()
                .Signal<Events::SceneActivated>(_ActiveScene);
        }
    }
}

void Arcadia::Project::_SnapshotEntities()
{
    for(auto [entity, comp] : _ActiveScene->GetComponentView<CameraComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->GetComponentView<LightComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->GetComponentView<ModelComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->GetComponentView<PhysicsComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->GetComponentView<TransformComponent>().each())
    {
        comp.Snapshot();
    }
}