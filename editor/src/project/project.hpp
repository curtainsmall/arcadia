#pragma once

#include<filesystem>
#include<memory>
#include<string>
#include<unordered_map>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"
#include"core/layer/layer.hpp"
#include"core/nlohmann_json_header.hpp"
#include"resource/scene/scene.hpp"

namespace arcadia
{
    struct ARCADIA_API project: arcadia::noncopyable
    {
    public:
        using self_type = project;
    public:
        std::unordered_map<std::string, arcadia::scene> scene_umap{};
        arcadia::scene* active_scene_ptr{};
    };

    struct ARCADIA_API project_layer: arcadia::layer_interface
    {
    public:
        project_layer();
        virtual ~project_layer();

        virtual auto on_event(const arcadia::event_base& event) -> bool override;
        virtual void on_update(delta_time_type delta_time) override;

    private:

    private:
        std::unique_ptr<arcadia::project> _project_ptr{};

    };
}
