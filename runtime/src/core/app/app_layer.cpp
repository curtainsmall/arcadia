#include"pch.hpp"
#include"app_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/util/conditional.hpp"
#include"core/version/version.hpp"

arcadia::app_layer::app_layer():
    arcadia::layer_interface("app")
{
    // Prepare app_config (either read from disk or use default value)
    try
    {
        auto ifs = arcadia::file::create_ifstream(arcadia::app_config::filepath);
        auto json = nlohmann::json::parse(ifs);

        auto& app_config = arcadia::app_config::instance();

        // Working directory
        app_config.working_directory = arcadia::to_filepath(json.at("working_directory"));

        // Graphic api
        auto& json_graphic_api = json.at("graphic_api");
        arcadia::version graphic_api_version{ json_graphic_api.at("version") };
        std::string graphic_api_type_str = json_graphic_api.at("type");
        app_config.graphic_api = arcadia::match<arcadia::graphic_api::type>(
            graphic_api_type_str,
            []()
        {
            return arcadia::graphic_api::type{};
        },
            "opengl"s,
            [&]()
        {
            return arcadia::graphic_api::opengl{ graphic_api_version };
        },
            "directx"s,
            [&]()
        {
            return arcadia::graphic_api::directx{ graphic_api_version };
        },
            "vulkan"s,
            [&]()
        {
            return arcadia::graphic_api::vulkan{ graphic_api_version };
        }
        );

        // Window
        auto& json_window = json.at("window");
        app_config.window_pos = arcadia::ivec2::from_json(json_window.at("pos"));
        app_config.window_size = arcadia::ivec2::from_json(json_window.at("size"));
        app_config.window_max_size = arcadia::ivec2::from_json(json_window.at("max_size"));
        app_config.window_min_size = arcadia::ivec2::from_json(json_window.at("min_size"));
        app_config.window_multisample_count = json_window.at("multisample_count");
        app_config.window_title = json_window.at("title");
        app_config.window_maxmized = json_window.at("maxmized");
    }
    catch(const std::ios_base::failure&)
    {
        // Use default value
    }
}

arcadia::app_layer::~app_layer()
{
    auto& app_config = arcadia::app_config::instance();

    auto json = nlohmann::json::object();

    // Working directory
    json.push_back({ "working_directory",app_config.working_directory.generic_string() });

    // Graphic api
    const auto [graphic_api_type_str, json_version] = arcadia::match<std::tuple<std::string, nlohmann::json>>(
        app_config.graphic_api,
        [&](const arcadia::graphic_api::opengl& gl)
    {
        return std::make_tuple("opengl"s, gl.version.to_json());
    },
        [&](const arcadia::graphic_api::directx& dx)
    {
        return std::make_tuple("directx"s, dx.version.to_json());
    },
        [&](const arcadia::graphic_api::vulkan& vk)
    {
        return std::make_tuple("vulkan"s, vk.version.to_json());
    }
    );
    json.push_back(
        { "graphic_api",{
            {"version", json_version},
            {"type",graphic_api_type_str}
            }
        }
    );

    // Window
    json.push_back(
        { "window",{
            {"pos", arcadia::ivec2::to_json(app_config.window_pos)},
            {"size",arcadia::ivec2::to_json(app_config.window_size)},
            {"max_size",arcadia::ivec2::to_json(app_config.window_max_size)},
            {"min_size",arcadia::ivec2::to_json(app_config.window_min_size)},
            {"multisample_count",app_config.window_multisample_count},
            {"title",app_config.window_title},
            {"maxmized",app_config.window_maxmized}
            }
        }
    );

    auto ofs = arcadia::file::create_ofstream(arcadia::app_config::filepath);
    ofs << std::setw(4) << json;
}

