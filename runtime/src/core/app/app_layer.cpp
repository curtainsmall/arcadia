#include"pch.hpp"

#include"app_layer.hpp"

#include"core/app/app_config.hpp"
#include"core/file/file.hpp"
#include"core/match.hpp"
#include"core/math.hpp"
#include"core/nlohmann_json_header.hpp"
#include"core/version/version.hpp"

Arcadia::iAppLayer::iAppLayer() :
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
            Version graphic_api_version(json_graphic_api.at("version"));
            std::string graphic_api_type_string = json_graphic_api.at("type");
            app_config.GraphicApi = Match<GraphicApi::Type>(
                graphic_api_type_string,
                []()
            {
                return GraphicApi::Type();
            },
                "opengl",
                [&]()
            {
                return GraphicApi::Opengl(graphic_api_version);
            },
                "directx",
                [&]()
            {
                return GraphicApi::Directx(graphic_api_version);
            },
                "vulkan",
                [&]()
            {
                return GraphicApi::Vulkan(graphic_api_version);
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
            const auto& json_window           = json.at("window");
            app_config.WindowPosition         = GlmInt32Vec2::FromJson(json_window.value("pos", GlmInt32Vec2::ToJson(app_config.WindowPosition)));
            app_config.WindowSize             = GlmInt32Vec2::FromJson(json_window.value("size", GlmInt32Vec2::ToJson(app_config.WindowSize)));
            app_config.WindowSizeMax          = GlmInt32Vec2::FromJson(json_window.value("max_size", GlmInt32Vec2::ToJson(app_config.WindowSizeMax)));
            app_config.WindowSizeMin          = GlmInt32Vec2::FromJson(json_window.value("min_size", GlmInt32Vec2::ToJson(app_config.WindowSizeMin)));
            app_config.WindowMultisampleCount = json_window.value("multisample_count", app_config.WindowMultisampleCount);
            app_config.WindowTitle            = json_window.value("title", app_config.WindowTitle);
            app_config.WindowMaxmized         = json_window.value("maxmized", app_config.WindowMaxmized);
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

Arcadia::iAppLayer::~iAppLayer()
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
        return std::make_tuple(std::string("opengl"), gl.Version.ToJson());
    },
        [&](const GraphicApi::Directx& dx)
    {
        return std::make_tuple(std::string("directx"), dx.Version.ToJson());
    },
        [&](const GraphicApi::Vulkan& vk)
    {
        return std::make_tuple(std::string("vulkan"), vk.Version.ToJson());
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
            {"pos", GlmInt32Vec2::ToJson(app_config.WindowPosition)},
            {"size",GlmInt32Vec2::ToJson(app_config.WindowSize)},
            {"max_size",GlmInt32Vec2::ToJson(app_config.WindowSizeMax)},
            {"min_size",GlmInt32Vec2::ToJson(app_config.WindowSizeMin)},
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
    for(const auto& id_string : app_config.ImguiOpenedWindowIdStrings)
    {
        json.at("imgui")
            .at("opened_window_id_strs")
            .push_back(id_string);
    }

    auto ofs = File::CreateOfstream(AppConfig::Filepath);
    ofs << std::setw(4) << json;
}