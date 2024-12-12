#include"pch.hpp"

#include"app_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/version/version.hpp"

iAppLayer::iAppLayer() :
    iLayer("app")
{
    // Prepare AppConfig (either read from disk or use default value)
    try
    {
        auto ifs = File::CreateIfstream(AppConfig::Filepath);
        auto json = nlohmann::json::parse(ifs);

        auto& app_config = AppConfig::Instance();

        // Working directory
        app_config.WorkingDirectory = ToFilepath(json.value("working_directory", app_config.WorkingDirectory.generic_string()));

        // Graphic api
        try
        {
            const auto& json_graphic_api = json.at("graphic_api");
            Version graphic_api_version{ json_graphic_api.at("version") };
            std::string graphic_api_type_str = json_graphic_api.at("type");
            app_config.GraphicApi = Match<GraphicApi::Type>(
                graphic_api_type_str,
                []()
            {
                return GraphicApi::Type{};
            },
                "opengl"s,
                [&]()
            {
                return GraphicApi::Opengl{ graphic_api_version };
            },
                "directx"s,
                [&]()
            {
                return GraphicApi::Directx{ graphic_api_version };
            },
                "vulkan"s,
                [&]()
            {
                return GraphicApi::Vulkan{ graphic_api_version };
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
            app_config.WindowPosition               = IntVec2::FromJson(json_window.value("pos", IntVec2::ToJson(app_config.WindowPosition)));
            app_config.WindowSize              = IntVec2::FromJson(json_window.value("size", IntVec2::ToJson(app_config.WindowSize)));
            app_config.WindowSizeMax          = IntVec2::FromJson(json_window.value("max_size", IntVec2::ToJson(app_config.WindowSizeMax)));
            app_config.WindowSizeMin          = IntVec2::FromJson(json_window.value("min_size", IntVec2::ToJson(app_config.WindowSizeMin)));
            app_config.WindowMultisampleCount = json_window.value("multisample_count", app_config.WindowMultisampleCount);
            app_config.WindowTitle             = json_window.value("title", app_config.WindowTitle);
            app_config.WindowMaxmized          = json_window.value("maxmized", app_config.WindowMaxmized);
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
                app_config.ImguiOpenedWindowIdStrings.emplace(id_strs);
            }
            app_config.UiScale = json_imgui.at("ui_scale");
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
    auto& app_config = AppConfig::Instance();

    auto json = nlohmann::json::object();

    // Working directory
    json.push_back({ "working_directory",app_config.WorkingDirectory.generic_string() });

    // Graphic api
    const auto [graphic_api_type_str, json_version] = Match<std::tuple<std::string, nlohmann::json>>(
        app_config.GraphicApi,
        [&](const GraphicApi::Opengl& gl)
    {
        return std::make_tuple("opengl"s, gl.Version.ToJson());
    },
        [&](const GraphicApi::Directx& dx)
    {
        return std::make_tuple("directx"s, dx.Version.ToJson());
    },
        [&](const GraphicApi::Vulkan& vk)
    {
        return std::make_tuple("vulkan"s, vk.Version.ToJson());
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
            {"pos", IntVec2::ToJson(app_config.WindowPosition)},
            {"size",IntVec2::ToJson(app_config.WindowSize)},
            {"max_size",IntVec2::ToJson(app_config.WindowSizeMax)},
            {"min_size",IntVec2::ToJson(app_config.WindowSizeMin)},
            {"multisample_count",app_config.WindowMultisampleCount},
            {"title",app_config.WindowTitle},
            {"maxmized",app_config.WindowMaxmized}
            }
        }
    );

    // ImGui
    json.push_back(
        { "imgui",{
            {"opened_window_id_strs",nlohmann::json::array()},
            {"ui_scale",app_config.UiScale}
            }
        }
    );
    for(const auto& id_str : app_config.ImguiOpenedWindowIdStrings)
    {
        json.at("imgui")
            .at("opened_window_id_strs")
            .push_back(id_str);
    }

    auto ofs = File::CreateOfstream(AppConfig::Filepath);
    ofs << std::setw(4) << json;
}