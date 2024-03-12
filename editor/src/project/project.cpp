#include "project.hpp"

arcadia::project::project(nlohmann::json& json):
    _name(json.at("name")),
    viewport_camera(json.at("viewport_camera"))
{
    for(const auto& json_scene : json.at("scenes"))
    {
        scene_sptr_umap.try_emplace(json_scene.at("name"), std::make_shared<arcadia::scene>(json_scene));
    }

    set_active_scene(json.at("active_scene_name"));
}

auto arcadia::project::to_json() const -> nlohmann::json
{
    nlohmann::json json{
        {"name",get_name()},
        {"scenes",nlohmann::json::array()},
        {"active_scene_name",has_active_scene() ? get_active_scene().get_name() : ""s},
        {"viewport_camera",viewport_camera.to_json()}
    };

    for(const auto& [name, scene_sptr] : scene_sptr_umap)
    {
        json.at("scenes")
            .push_back(scene_sptr->to_json());
    }

    return json;
}

auto arcadia::project::is_modified() const -> bool
{
    if(_modified)
    {
        return true;
    }
    for(const auto& [name, scene_sptr] : scene_sptr_umap)
    {
        if(scene_sptr->is_modified())
        {
            return true;
        }
    }
    return false;
}

void arcadia::project::set_modified(bool modified, bool recursively)
{
    _modified = modified;
    if(recursively)
    {
        for(auto& [name, scene_sptr] : scene_sptr_umap)
        {
            scene_sptr->set_modified(modified);
        }
    }
}

auto arcadia::project::has_active_scene() const -> bool
{
    return !_active_scene_wptr.expired();
}

auto arcadia::project::get_active_scene() -> arcadia::scene&
{
    if(!has_active_scene())
    {
        throw no_active_scene{};
    }
    // If scene is modified, it will record it internally so we does not need to change _modified here
    return *_active_scene_wptr.lock();
}

auto arcadia::project::get_active_scene() const -> const arcadia::scene&
{
    if(!has_active_scene())
    {
        throw no_active_scene{};
    }
    return *_active_scene_wptr.lock();
}

auto arcadia::project::set_active_scene(const std::string& name) -> std::weak_ptr<arcadia::scene>&
{
    auto has_active_scene = !_active_scene_wptr.expired();
    auto active_scene_sptr = _active_scene_wptr.lock();

    auto is_same_scene = has_active_scene && name == active_scene_sptr->get_name();

    if(!is_same_scene)
    {
        if(has_active_scene)
        {
            _active_scene_wptr.reset();
            arcadia::event_queue::instance()
                .signal<arcadia::event::scene_deactivated>();
        }

        if(name.size() && scene_sptr_umap.find(name) != scene_sptr_umap.end())
        {
            _active_scene_wptr = scene_sptr_umap.at(name);
            arcadia::event_queue::instance()
                .signal<arcadia::event::scene_activated>(_active_scene_wptr);
        }
    }

    set_modified(true);
    return _active_scene_wptr;
}
