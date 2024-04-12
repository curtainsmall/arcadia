#include "project.hpp"

#include"resource/component/camera_component/camera_component.hpp"
#include"resource/component/light_component/light_component.hpp"
#include"resource/component/model_component/model_component.hpp"
#include"resource/component/physics_component/physics_component.hpp"

Arcadia::Project::Project(nlohmann::json& json):
    _Name(json.at("name")),
    ViewportCamera(json.at("viewport_camera"))
{
    for(const auto& json_scene : json.at("scenes"))
    {
        umapSceneSptr.try_emplace(json_scene.at("name"), std::make_shared<Arcadia::Scene>(json_scene));
    }

    SetActiveScene(json.at("active_scene_name"));
}

auto Arcadia::Project::ToJson() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",GetName()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",HasActiveScene() ? GetActiveScene().GetName() : ""s},
        {"viewport_camera",ViewportCamera.ToJson()}
    };

    for(const auto& [name, scene_sptr] : umapSceneSptr)
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
    return !!_spActiveScene;
}

auto Arcadia::Project::GetActiveScene() -> Arcadia::Scene&
{
    ARCADIA_ASSERT(HasActiveScene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_spActiveScene;
}

auto Arcadia::Project::GetActiveScene() const -> const Arcadia::Scene&
{
    ARCADIA_ASSERT(HasActiveScene());
    return *_spActiveScene;
}

void Arcadia::Project::SetActiveScene(const std::string& name)
{
    auto is_same_scene = _spActiveScene && name == _spActiveScene->GetName();

    if(!is_same_scene)
    {
        if(_spActiveScene)
        {
            _spActiveScene.reset();
            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::SceneDeactivated>();
        }

        if(!name.empty() && umapSceneSptr.find(name) != umapSceneSptr.end())
        {
            _spActiveScene = umapSceneSptr.at(name);

            // Snapshot the scene but not put it into memento list
            _SnapshotEntities();

            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::SceneActivated>(_spActiveScene);
        }
    }
}

void Arcadia::Project::_SnapshotEntities()
{
    for(auto [entity, comp] : _spActiveScene->View<Arcadia::CameraComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _spActiveScene->View<Arcadia::LightComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _spActiveScene->View<Arcadia::ModelComponent>().each())
    {
        comp.Snapshot();
    }
    for(auto [entity, comp] : _spActiveScene->View<Arcadia::PhysicsComponent>().each())
    {
        comp.Snapshot();
    }
}
