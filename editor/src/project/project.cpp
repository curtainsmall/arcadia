#include "project.hpp"

#include "core/assert.hpp"
#include "core/command/command.hpp"

#include "project/project_events.hpp"

Arcadia::Project::Project(nlohmann::json& json):
    _Name(json.at("name"))
{
    try
    {
        for(const nlohmann::json& json_scene : json.at("scenes"))
        {
            _SceneStorage.try_emplace(json_scene.at("name"), std::make_shared<Scene>(json_scene));
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

    for(const auto& [name, scene_sptr] : _SceneStorage)
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
    return *_spActiveScene;
}

auto Arcadia::Project::GetActiveScene() const -> const Scene&
{
    ACDA_ASSERT(HasActiveScene());
    return *_spActiveScene;
}

void Arcadia::Project::SetActiveScene(const std::string& name)
{
    bool is_same_scene = _spActiveScene && name == _spActiveScene->GetName();

    if(!is_same_scene)
    {
        CommandList::Instance().Clear();

        if(_spActiveScene)
        {
            _spActiveScene.reset();
            EventQueue::Instance()
                .Signal<Events::SceneDeactivated>();
        }

        if(!name.empty() && _SceneStorage.find(name) != _SceneStorage.end())
        {
            _spActiveScene = _SceneStorage.at(name);

            EventQueue::Instance()
                .Signal<Events::SceneActivated>(_spActiveScene);
        }
    }
}

auto Arcadia::Project::HasScene() const -> bool
{
    return !_SceneStorage.empty();
}

auto Arcadia::Project::HasScene(const std::string& name) const -> bool
{
    return _SceneStorage.contains(name);
}

auto Arcadia::Project::GetScene(const std::string& name) -> Scene&
{
    ACDA_ASSERT(HasScene(name));
    return *_SceneStorage.at(name);
}

auto Arcadia::Project::GetScene(const std::string& name) const -> const Scene&
{
    ACDA_ASSERT(HasScene(name));
    return *_SceneStorage.at(name);
}

void Arcadia::Project::CreateScene(const std::string& name)
{
    ACDA_ASSERT(!HasScene(name));

    _SceneStorage.try_emplace(
        name,
        std::make_shared<Scene>(name)
    );
}

void Arcadia::Project::DestroyScene(const std::string& name)
{
    ACDA_ASSERT(HasScene(name));

    _SceneStorage.erase(name);
}

void Arcadia::Project::RenameScene(const std::string& name, const std::string& new_name)
{
    ACDA_ASSERT(HasScene(name));
    ACDA_ASSERT(!HasScene(new_name));

    SceneStorageType::node_type node = _SceneStorage.extract(name);
    node.key() = new_name;
    node.mapped()->SetName(new_name);
    _SceneStorage.insert(std::move(node));
}

auto Arcadia::Project::GetSceneStorage() const -> const SceneStorageType&
{
    return _SceneStorage;
}