#include "runtime_layer.hpp"

#include "core/runtime_config.hpp"
#include "core/match.hpp"
#include "core/function.hpp"
#include "function/window/window_layer.hpp"
#include "function/physics/physics_layer.hpp"
#include "function/render/renderer_layer.hpp"
#include "function/script/script_layer.hpp"
#include "resource/scene_layer.hpp"

Arcadia::RuntimeLayer::RuntimeLayer():
    LayerInterface("runtime")
{
    // Prepare RuntimeConfig(either read from disk or use default value)
    try
    {
        std::ifstream ifs(RuntimeConfig::Filepath);
        nlohmann::json json{};
        // nlohmann::json::parse_error may be thrown here, in which case all fields in RuntimeConfig class will be their default values
        ifs >> json;

        RuntimeConfig& runtime_config = RuntimeConfig::Instance();

        // Working directory
        runtime_config.WorkingDirectory = ToFilepath(json.value("working_directory", runtime_config.WorkingDirectory.generic_string()));

        // Graphic api
        try
        {
            const nlohmann::json& json_graphic_api = json.at("graphic_api");
            Version graphic_api_version(json_graphic_api.at("version"));
            std::string graphic_api_type_string = json_graphic_api.at("type");
            runtime_config.GraphicApi = Match<GraphicApi::Type>(
                graphic_api_type_string,
                []()
                {
                    // Default graphic API
                    return GraphicApi::Opengl(Version(4, 6, 0));
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
            // Use default value defined in RuntimeConfig
        }

        // Window
        try
        {
            const nlohmann::json& json_window = json.at("window");
            runtime_config.WindowPosition = Glm::Int32Vec2_FromJson(json_window.value("pos", Glm::Int32Vec2_ToJson(runtime_config.WindowPosition)));
            runtime_config.WindowSize = Glm::Int32Vec2_FromJson(json_window.value("size", Glm::Int32Vec2_ToJson(runtime_config.WindowSize)));
            runtime_config.WindowSizeMax = Glm::Int32Vec2_FromJson(json_window.value("max_size", Glm::Int32Vec2_ToJson(runtime_config.WindowSizeMax)));
            runtime_config.WindowSizeMin = Glm::Int32Vec2_FromJson(json_window.value("min_size", Glm::Int32Vec2_ToJson(runtime_config.WindowSizeMin)));
            runtime_config.WindowMultisampleCount = json_window.value("multisample_count", runtime_config.WindowMultisampleCount);
            runtime_config.WindowTitle = json_window.value("title", runtime_config.WindowTitle);
            runtime_config.WindowMaxmized = json_window.value("maxmized", runtime_config.WindowMaxmized);
        }
        catch(nlohmann::json::out_of_range)
        {
            // Use default value defined in RuntimeConfig
        }

        // ImGui
        try
        {
            const nlohmann::json& json_imgui = json.at("imgui");
            for(const auto& id_strs : json_imgui.value("opened_window_id_strs", nlohmann::json::array()))
            {
                runtime_config.ImguiOpenedWindowIdStrings.emplace(id_strs);
            }
            runtime_config.UiScale = json_imgui.at("ui_scale");
        }
        catch(nlohmann::json::out_of_range)
        {
            // Use default value defined in RuntimeConfig
        }
    }
    catch(const nlohmann::json::parse_error&)
    {
        // Use default value for all fields of RuntimeConfig defined in the class
    }

    LayerStack& layer_stack = LayerStack::Instance();
    RuntimeConfig& runtime_config = RuntimeConfig::Instance();

    // Window layer
    layer_stack.PushLayer<WindowLayer>(
        runtime_config.WindowSize,
        runtime_config.WindowTitle,
        runtime_config.WindowMultisampleCount
    );

    // Physics layer
    layer_stack.PushLayer<PhysicsLayer>();

    // Scene layer
    layer_stack.PushLayer<SceneLayer>();

    // Renderer layer
    layer_stack.PushLayer<RendererLayer>(
        runtime_config.GraphicApi,
        runtime_config.WorkingDirectory
    );

    layer_stack.PushLayer<ScriptLayer>();
}

Arcadia::RuntimeLayer::~RuntimeLayer()
{
    RuntimeConfig& runtime_config = RuntimeConfig::Instance();

    nlohmann::json json = nlohmann::json::object();

    // Working directory
    json.push_back({ "working_directory",runtime_config.WorkingDirectory.generic_string() });

    // Graphic api
    const auto [graphic_api_type_str, json_version] = MatchVariant<std::tuple<std::string, nlohmann::json>>(
        runtime_config.GraphicApi,
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
            {"pos", Glm::Int32Vec2_ToJson(runtime_config.WindowPosition)},
            {"size",Glm::Int32Vec2_ToJson(runtime_config.WindowSize)},
            {"max_size",Glm::Int32Vec2_ToJson(runtime_config.WindowSizeMax)},
            {"min_size",Glm::Int32Vec2_ToJson(runtime_config.WindowSizeMin)},
            {"multisample_count",runtime_config.WindowMultisampleCount},
            {"title",runtime_config.WindowTitle},
            {"maxmized",runtime_config.WindowMaxmized}
            }
        }
    );

    // ImGui
    json.push_back(
        { "imgui",{
            {"opened_window_id_strs",nlohmann::json::array()},
            {"ui_scale",runtime_config.UiScale}
            }
        }
    );
    for(const std::string& id_string : runtime_config.ImguiOpenedWindowIdStrings)
    {
        json.at("imgui")
            .at("opened_window_id_strs")
            .push_back(id_string);
    }

    std::ofstream ofs(RuntimeConfig::Filepath);
    ofs << std::setw(4) << json;
}

void Arcadia::RuntimeLayer::OnEvent(EventBase& event)
{
    EventDispatcher(event)
        .Dispatch<Events::RuntimeStart>(ACDA_BIND_MEMBER_FN(_OnRuntimeStart))
        .Dispatch<Events::RuntimeStop>(ACDA_BIND_MEMBER_FN(_OnRuntimeStop))
        .IsDispatched();
}

void Arcadia::RuntimeLayer::OnUpdate()
{
    _DeltaTime = _Timer.Segment();
}

auto Arcadia::RuntimeLayer::IsRunning() const -> bool
{
    return _Running;
}

auto Arcadia::RuntimeLayer::GetDeltaTime() const -> std::chrono::nanoseconds
{
    return _DeltaTime;
}

void Arcadia::RuntimeLayer::_OnRuntimeStart(Events::RuntimeStart& e)
{
    _Running = true;
}

void Arcadia::RuntimeLayer::_OnRuntimeStop(Events::RuntimeStop& e)
{
    _Running = false;
}