#include"pch.hpp"

#include"app_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/version/version.hpp"

iAppLayer::iAppLayer():
    iLayer("app")
{
    // Prepare AppConfig (either read from disk or use default value)
    try
    {
        auto ifs = File::create_ifstream(AppConfig::filepath);
        auto json = nlohmann::json::parse(ifs);

        auto& app_config = AppConfig::instance();

        // Working directory
        app_config.working_directory = to_filepath(json.value("working_directory", app_config.working_directory.generic_string()));

        // Graphic api
        try
        {
            const auto& json_graphic_api = json.at("graphic_api");
            Version graphic_api_version{ json_graphic_api.at("version") };
            std::string graphic_api_type_str = json_graphic_api.at("type");
            app_config.graphic_api = match<graphic_api::Type>(
                graphic_api_type_str,
                []()
            {
                return graphic_api::Type{};
            },
                "opengl"s,
                [&]()
            {
                return graphic_api::Opengl{ graphic_api_version };
            },
                "directx"s,
                [&]()
            {
                return graphic_api::Directx{ graphic_api_version };
            },
                "vulkan"s,
                [&]()
            {
                return graphic_api::Vulkan{ graphic_api_version };
            }
            );
        }
        catch(nlohmann::json::out_of_range)
        {
            // Use default value
        }

        // Window
        try
        {
            const auto& json_window             = json.at("window");
            app_config.window_pos               = ivec2::from_json(json_window.value("pos", ivec2::to_json(app_config.window_pos)));
            app_config.window_size              = ivec2::from_json(json_window.value("size", ivec2::to_json(app_config.window_size)));
            app_config.window_size_max          = ivec2::from_json(json_window.value("max_size", ivec2::to_json(app_config.window_size_max)));
            app_config.window_size_min          = ivec2::from_json(json_window.value("min_size", ivec2::to_json(app_config.window_size_min)));
            app_config.window_multisample_count = json_window.value("multisample_count", app_config.window_multisample_count);
            app_config.window_title             = json_window.value("title", app_config.window_title);
            app_config.window_maxmized          = json_window.value("maxmized", app_config.window_maxmized);
        }
        catch(nlohmann::json::out_of_range)
        {
            // Use default value
        }

        // ImGui
        try
        {
            const auto& json_imgui = json.at("imgui");
            for(const auto& id_strs : json_imgui.value("opened_window_id_strs", nlohmann::json::array()))
            {
                app_config.imgui_opened_window_id_strs.emplace(id_strs);
            }
        }
        catch(nlohmann::json::out_of_range)
        {
            // Use default value
        }
    }
    catch(const std::ios_base::failure&)
    {
        // Use default value
    }
}

iAppLayer::~iAppLayer()
{
    auto& app_config = AppConfig::instance();

    auto json = nlohmann::json::object();

    // Working directory
    json.push_back({ "working_directory",app_config.working_directory.generic_string() });

    // Graphic api
    const auto [graphic_api_type_str, json_version] = match<std::tuple<std::string, nlohmann::json>>(
        app_config.graphic_api,
        [&](const graphic_api::Opengl& gl)
    {
        return std::make_tuple("opengl"s, gl.version.to_json());
    },
        [&](const graphic_api::Directx& dx)
    {
        return std::make_tuple("directx"s, dx.version.to_json());
    },
        [&](const graphic_api::Vulkan& vk)
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
            {"pos", ivec2::to_json(app_config.window_pos)},
            {"size",ivec2::to_json(app_config.window_size)},
            {"max_size",ivec2::to_json(app_config.window_size_max)},
            {"min_size",ivec2::to_json(app_config.window_size_min)},
            {"multisample_count",app_config.window_multisample_count},
            {"title",app_config.window_title},
            {"maxmized",app_config.window_maxmized}
            }
        }
    );

    // ImGui
    json.push_back(
        { "imgui",{
            {"opened_window_id_strs",nlohmann::json::array()}
            }
        }
    );
    for(const auto& id_str : app_config.imgui_opened_window_id_strs)
    {
        json.at("imgui")
            .at("opened_window_id_strs")
            .push_back(id_str);
    }

    auto ofs = File::create_ofstream(AppConfig::filepath);
    ofs << std::setw(4) << json;
}

