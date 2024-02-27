#pragma once

#include<memory>
#include<string>

#include"core/app/app_config.hpp"
#include"core/app/app_context.hpp"
#include"core/app/app_layer.hpp"
#include"core/base.hpp"
#include"core/file/file.hpp"
#include"core/layer/layer.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/util/conditional.hpp"

extern auto create_application_uptr() -> std::unique_ptr<arcadia::app_layer>;

auto main(
    int argc,
    const char** argv
) -> int
{
    // Prepare app_config (either read from disk or use default value)
    try
    {
        auto ifs = arcadia::file::create_ifstream();
        ifs.open(".arcadiacfg");
        auto json = json::parse(ifs);

        auto& app_config = arcadia::app_config::instance();

        // Working directory
        app_config.working_directory = json.at("working_directory").dump();

        // Graphic api
        auto& json_graphic_api = json.at("graphic_api");
        arcadia::version graphic_api_version{};
        auto& json_graphic_api_version = json_graphic_api.at("version");
        graphic_api_version.major = json_graphic_api_version.at("major");
        graphic_api_version.minor = json_graphic_api_version.at("minor");
        graphic_api_version.patch = json_graphic_api_version.at("patch");
        std::string graphic_api_type_str = json.at("type");
        app_config.graphic_api = arcadia::match<arcadia::graphic_api::type>(
            graphic_api_type_str,
            []() -> arcadia::graphic_api::type
        {
            return {};
        },
            "opengl"s,
            [&]() -> arcadia::graphic_api::type
        {
            return arcadia::graphic_api::opengl{ graphic_api_version };
        },
            "directx"s,
            [&]() -> arcadia::graphic_api::type
        {
            return arcadia::graphic_api::directx{ graphic_api_version };
        },
            "vulkan"s,
            [&]() -> arcadia::graphic_api::type
        {
            return arcadia::graphic_api::vulkan{ graphic_api_version };
        }
        );
    }
    catch(const std::ios_base::failure&)
    {
    }

    // Add app_layer
    auto& layer_stack = arcadia::layer_stack::instance();
    layer_stack.push_layer_at<arcadia::app_layer>(layer_stack.size() - 1, arcadia::create_application_uptr());

    // Main loop
    auto& app_context = arcadia::app_context::instance();
    while(app_context.running)
    {
        // Process event 
        auto& event_queue = arcadia::event_queue::instance();
        event_queue.swap_queue();
        while(event_queue.size())
        {
            auto& event = event_queue.read();

            for(auto& layer_uptr : std::ranges::reverse_view{ arcadia::layer_stack::instance() })
            {
                if(layer_uptr->on_event(event))
                {
                    break;
                }
            }
            event_queue.pop();
        }

        // Updates
        for(auto& layer_uptr : arcadia::layer_stack::instance())
        {
            layer_uptr->on_update(app_context.timer.since_last());
        }
    }

    // Clear layer_stack
    layer_stack.pop_all();
    return 0;
}
