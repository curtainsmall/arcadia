#pragma once

#include<functional>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

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
    class iImguiWindow
    {
    public:
        using SelfType = iImguiWindow;
    public:
        iImguiWindow(
            bool open = false,
            const std::string& title={}
        ) :
            _Opened(open),
            _Title(title)
        {}
        virtual ~iImguiWindow() = default;

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

    template<typename ImGuiWindow>
    concept cImguiWindow = requires{
        std::derived_from<ImGuiWindow, iImguiWindow>;
        {
            ImGuiWindow::GetIdStringStatic()
        } -> std::same_as<std::string>;
    };
}