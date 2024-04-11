#include "project.hpp"

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
    return !_wpActiveScene.expired();
}

auto Arcadia::Project::GetActiveScene() -> Arcadia::Scene&
{
    ARCADIA_ASSERT(HasActiveScene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_wpActiveScene.lock();
}

auto Arcadia::Project::GetActiveScene() const -> const Arcadia::Scene&
{
    ARCADIA_ASSERT(HasActiveScene());
    return *_wpActiveScene.lock();
}

auto Arcadia::Project::SetActiveScene(const std::string& name) -> std::weak_ptr<Arcadia::Scene>&
{
    auto has_active_scene = !_wpActiveScene.expired();
    auto active_scene_sptr = _wpActiveScene.lock();

    auto is_same_scene = has_active_scene && name == active_scene_sptr->GetName();

    if(!is_same_scene)
    {
        if(has_active_scene)
        {
            _wpActiveScene.reset();
            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::SceneDeactivated>();
        }

        if(!name.empty() && umapSceneSptr.find(name) != umapSceneSptr.end())
        {
            _wpActiveScene = umapSceneSptr.at(name);
            Arcadia::EventQueue::Instance()
                .Signal<Arcadia::Event::SceneActivated>(_wpActiveScene);
        }
    }

    return _wpActiveScene;
}
