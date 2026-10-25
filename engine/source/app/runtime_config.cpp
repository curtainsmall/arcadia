#include "app/runtime_config.hpp"

#include <fstream>

namespace Arcadia
{
    auto RuntimeConfig::ToJson() const -> nlohmann::json
    {
        nlohmann::json json{};

        json["working_directory"] = WorkingDirectory.generic_string();

        json["graphic_api"] = nlohmann::json{
            { "type",GraphicApi },
            { "version",nlohmann::json::array({ GraphicApiMajor,GraphicApiMinor,GraphicApiPatch }) }
        };

        // Keys kept identical to the legacy file: "pos", "min_size", "max_size"
        // and the historical "maxmized" spelling.
        json["window"] = nlohmann::json{
            { "pos",nlohmann::json::array({ WindowPositionX,WindowPositionY }) },
            { "size",nlohmann::json::array({ WindowWidth,WindowHeight }) },
            { "max_size",nlohmann::json::array({ WindowMaxWidth,WindowMaxHeight }) },
            { "min_size",nlohmann::json::array({ WindowMinWidth,WindowMinHeight }) },
            { "multisample_count",MultisampleCount },
            { "title",WindowTitle },
            { "maxmized",WindowMaximized }
        };

        json["imgui"] = nlohmann::json{
            { "opened_window_id_strs",OpenedWindowIds },
            { "ui_scale",UiScale }
        };

        return json;
    }

    void RuntimeConfig::FromJson(const nlohmann::json& json)
    {
        const auto read_vec2 = [](const nlohmann::json& object, const char* key, std::int32_t& x, std::int32_t& y) -> void
        {
            auto iter = object.find(key);
            if(iter != object.end() && iter->is_array() && iter->size() >= 2)
            {
                x = iter->at(0).get<std::int32_t>();
                y = iter->at(1).get<std::int32_t>();
            }
        };

        WorkingDirectory = std::filesystem::path{ json.value("working_directory", WorkingDirectory.generic_string()) };

        if(auto iter = json.find("graphic_api"); iter != json.end() && iter->is_object())
        {
            const nlohmann::json& json_graphic_api = *iter;
            GraphicApi = json_graphic_api.value("type", GraphicApi);
            if(auto version = json_graphic_api.find("version"); version != json_graphic_api.end() && version->is_array() && version->size() >= 3)
            {
                GraphicApiMajor = version->at(0).get<std::int32_t>();
                GraphicApiMinor = version->at(1).get<std::int32_t>();
                GraphicApiPatch = version->at(2).get<std::int32_t>();
            }
        }

        if(auto iter = json.find("window"); iter != json.end() && iter->is_object())
        {
            const nlohmann::json& json_window = *iter;
            read_vec2(json_window, "pos", WindowPositionX, WindowPositionY);
            read_vec2(json_window, "size", WindowWidth, WindowHeight);
            read_vec2(json_window, "max_size", WindowMaxWidth, WindowMaxHeight);
            read_vec2(json_window, "min_size", WindowMinWidth, WindowMinHeight);
            MultisampleCount = json_window.value("multisample_count", MultisampleCount);
            WindowTitle = json_window.value("title", WindowTitle);
            WindowMaximized = json_window.value("maxmized", WindowMaximized);
        }

        if(auto iter = json.find("imgui"); iter != json.end() && iter->is_object())
        {
            const nlohmann::json& json_imgui = *iter;
            UiScale = json_imgui.value("ui_scale", UiScale);
            if(auto opened = json_imgui.find("opened_window_id_strs"); opened != json_imgui.end() && opened->is_array())
            {
                OpenedWindowIds.clear();
                for(const nlohmann::json& id_string : *opened)
                {
                    if(id_string.is_string())
                    {
                        OpenedWindowIds.emplace(id_string.get<std::string>());
                    }
                }
            }
        }
    }

    auto RuntimeConfig::Load(const std::filesystem::path& filepath) -> bool
    {
        std::ifstream stream{ filepath };
        if(!stream.is_open())
        {
            return false;
        }

        try
        {
            nlohmann::json json{};
            stream >> json;
            FromJson(json);
        }
        catch(const nlohmann::json::exception&)
        {
            return false;
        }
    }

    auto RuntimeConfig::Save(const std::filesystem::path& filepath) const -> bool
    {
        std::ofstream stream{ filepath };
        if(!stream.is_open())
        {
            return false;
        }
        stream << ToJson().dump(4);
        return true;
    }
}
