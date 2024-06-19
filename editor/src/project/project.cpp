#include "project.hpp"

#include"resource/components/camera_component.hpp"
#include"resource/components/light_component.hpp"
#include"resource/components/model_component.hpp"
#include"resource/components/physics_component.hpp"
#include"resource/components/transform_component.hpp"

Project::Project(nlohmann::json& json):
    _name(json.at("name"))
{
    for(const auto& json_scene : json.at("scenes"))
    {
        scene_sptr_storage.try_emplace(json_scene.at("name"), std::make_shared<Scene>(json_scene));
    }

    set_active_scene(json.at("active_scene_name"));
}

auto Project::to_json() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",get_name()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",has_active_scene() ? get_active_scene().name : ""s}
    };

    for(const auto& [name, scene] : scene_sptr_storage)
    {
        json.at("scenes")
            .push_back(scene->to_json());
    }

    return json;
}

auto Project::get_name() const -> const std::string&
{
    return _name;
}

void Project::set_name(const std::string& name)
{
    _name = name;
}

auto Project::has_active_scene() const -> bool
{
    return !!_active_scene;
}

auto Project::get_active_scene() -> Scene&
{
    ACDA_ASSERT(has_active_scene());
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_active_scene;
}

auto Project::get_active_scene() const -> const Scene&
{
    ACDA_ASSERT(has_active_scene());
    return *_active_scene;
}

void Project::set_active_scene(const std::string& name)
{
    auto is_same_scene = _active_scene && name == _active_scene->name;

    if(!is_same_scene)
    {
        MementoList::instance().clear();

        if(_active_scene)
        {
            _active_scene.reset();
            EventQueue::instance()
                .signal<event::SceneDeactivated>();
        }

        if(!name.empty() && scene_sptr_storage.find(name) != scene_sptr_storage.end())
        {
            _active_scene = scene_sptr_storage.at(name);

            // Snapshot the scene but not put it into memento list
            _snapshot_entities();

            EventQueue::instance()
                .signal<event::SceneActivated>(_active_scene);
        }
    }
}

void Project::_snapshot_entities()
{
    for(auto [entity, comp] : _active_scene->view<CameraComponent>().each())
    {
        comp.snapshot();
    }
    for(auto [entity, comp] : _active_scene->view<LightComponent>().each())
    {
        comp.snapshot();
    }
    for(auto [entity, comp] : _active_scene->view<ModelComponent>().each())
    {
        comp.snapshot();
    }
    for(auto [entity, comp] : _active_scene->view<PhysicsComponent>().each())
    {
        comp.snapshot();
    }
    for(auto [entity, comp] : _active_scene->view<TransformComponent>().each())
    {
        comp.snapshot();
    }
}
