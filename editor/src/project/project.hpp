#pragma once

#include<filesystem>
#include<string>
#include<unordered_map>

#include"project/project_events.hpp"
#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct ARCADIA_API project: arcadia::noncopyable
    {
    public:
        ARCADIA_EXCEPTION(no_active_scene);

        using self_type = project;
    public:
        inline project(
            std::string name
        ):
            _name(name)
        {}
        ~project() = default;

        [[nodiscard]]
        inline auto get_name() const -> const std::string&
        {
            return _name;
        }
        inline auto set_name(const std::string& name) -> self_type&
        {
            _name = name;
            set_modified(true);
            return *this;
        }

        [[nodiscard]]
        inline auto is_modified() const -> bool
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
        inline void set_modified(bool modified, bool recursively = false)
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

        [[nodiscard]]
        inline auto has_active_scene() const -> bool
        {
            return !_active_scene_wptr.expired();
        }
        [[nodiscard]]
        inline auto get_active_scene() -> arcadia::scene&
        {
            if(!has_active_scene())
            {
                throw no_active_scene{};
            }
            // If scene is modified, it will record it internally so we does not need to change _modified here
            return *_active_scene_wptr.lock();
        }
        [[nodiscard]]
        inline auto get_active_scene() const -> const arcadia::scene&
        {
            if(!has_active_scene())
            {
                throw no_active_scene{};
            }
            return *_active_scene_wptr.lock();
        }
        inline auto set_active_scene(const std::string& name ={}) -> std::weak_ptr<arcadia::scene>&
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

    public:
        std::unordered_map<std::string, std::shared_ptr<arcadia::scene>> scene_sptr_umap{};
    private:
        std::weak_ptr<arcadia::scene> _active_scene_wptr{};
        bool _modified{ false };
        std::string _name;
    };
}
