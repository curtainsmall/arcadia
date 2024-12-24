#pragma once

#include<functional>
#include<string>

#include"core/event/event.hpp"
#include"platform/api_def.hpp"

// ImGui window ID should follow: Title###id so that we can change the title for a curtain window
#define ACDA_IMGUI_WINDOW_ID_STR_GETTERS(id_str) \
[[nodiscard]]\
static constexpr auto GetIdStringStatic() -> std::string\
{\
    return id_str;\
}\
[[nodiscard]]\
virtual auto GetIdString() const -> std::string override\
{\
    return GetIdStringStatic();\
}

namespace Arcadia
{
    class ImguiWindowInterface
    {
    public:
        using SelfType = ImguiWindowInterface;
    public:
        ImguiWindowInterface(
            bool open = false,
            const std::string& title={}
        ) :
            _Opened(open),
            _Title(title)
        {}
        virtual ~ImguiWindowInterface() = default;

        [[nodiscard]]
        auto Open() const -> bool
        {
            return _Opened;
        }

        [[nodiscard]]
        auto GetTitle() const -> const std::string&
        {
            return _Title;
        }

        virtual void OnEvent(EventBase& e)
        {}
        virtual void OnUpdate() = 0;
        virtual auto GetIdString() const->std::string = 0;

    protected:
        bool _Opened;
        std::string _Title{};
    };

    namespace Concepts
    {
        template<typename T>
        concept ImguiWindow = requires{
            std::derived_from<T, ImguiWindowInterface>;
            {
                T::GetIdStringStatic()
            } -> std::same_as<std::string>;
        };
    }
}