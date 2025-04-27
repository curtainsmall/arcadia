#include "project.hpp"

#include"core/assert.hpp"
#include"core/command/command.hpp"
#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Arcadia::Project::Project(nlohmann::json& json):
    _Name(json.at("name"))
{
    try
    {

        for(const nlohmann::json& json_scene : json.at("scenes"))
        {
            SceneStorage.try_emplace(json_scene.at("name"), std::make_shared<Scene>(json_scene));
        }

        SetActiveScene(json.at("active_scene_name"));
    }
    catch(const nlohmann::json::exception& e)
    {
        throw Exceptions::ProjectConstructionFailed(std::format("Json failure: {}", e.what()));
    }
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
    return !!_spActiveScene;
}

auto Arcadia::Project::GetActiveScene() -> Scene&
{
    ACDA_ASSERT(HasActiveScene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_spActiveScene;
}

auto Arcadia::Project::GetActiveScene() const -> const Scene&
{
    ACDA_ASSERT(HasActiveScene());
    return *_spActiveScene;
}

void Arcadia::Project::SetActiveScene(const std::string& name)
{
    auto is_same_scene = _spActiveScene && name == _spActiveScene->GetName();

    if(!is_same_scene)
    {
        CommandList::Instance().Clear();

        if(_spActiveScene)
        {
            _spActiveScene.reset();
            EventQueue::Instance()
                .Signal<Events::SceneDeactivated>();
        }

        if(!name.empty() && SceneStorage.find(name) != SceneStorage.end())
        {
            _spActiveScene = SceneStorage.at(name);

            EventQueue::Instance()
                .Signal<Events::SceneActivated>(_spActiveScene);
        }
    }
}
