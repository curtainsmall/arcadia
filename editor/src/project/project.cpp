#include "project.hpp"

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Project::Project(nlohmann::json& json):
    _Name(json.at("name"))
{
    for(const auto& json_scene : json.at("scenes"))
    {
        SceneSptrStorage.try_emplace(json_scene.at("name"), std::make_shared<Scene>(json_scene));
    }

    SetActiveScene(json.at("active_scene_name"));
}

auto Project::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",HasActiveScene() ? GetActiveScene().Name : ""s}
    };

    for(const auto& [name, scene] : SceneSptrStorage)
    {
        json.at("scenes")
            .push_back(scene->ToJson());
    }

    return json;
}

auto Project::GetName() const -> const std::string&
{
    return _Name;
}

void Project::SetName(const std::string& name)
{
    _Name = name;
}

auto Project::HasActiveScene() const -> bool
{
    return !!_ActiveScene;
}

auto Project::GetActiveScene() -> Scene&
{
    ACDA_ASSERT(HasActiveScene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_ActiveScene;
}

auto Project::GetActiveScene() const -> const Scene&
{
    ACDA_ASSERT(HasActiveScene());
    return *_ActiveScene;
}

void Project::SetActiveScene(const std::string& name)
{
    auto is_same_scene = _ActiveScene && name == _ActiveScene->Name;

    if(!is_same_scene)
    {
        MementoList::Instance().Clear();

        if(_ActiveScene)
        {
            _ActiveScene.reset();
            EventQueue::Instance()
                .Signal<Event::SceneDeactivated>();
        }

        if(!name.empty() && SceneSptrStorage.find(name) != SceneSptrStorage.end())
        {
            _ActiveScene = SceneSptrStorage.at(name);

            // Snapshot the scene but not put it into memento list
            _SnapshotEntities();

            EventQueue::Instance()
                .Signal<Event::SceneActivated>(_ActiveScene);
        }
    }
}

void Project::_SnapshotEntities()
{
    for(auto [entity, comp] : _ActiveScene->View<CameraComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->View<LightComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->View<ModelComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->View<PhysicsComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _ActiveScene->View<TransformComponent>().each())
    {
        comp.Snapshot();
    }
}
