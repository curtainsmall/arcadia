#pragma once

#include<functional>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"

#define ARCADIA_IMGUI_WINDOW_ID_STR_GETTERS(id_str) \
[[nodiscard]]\
static constexpr auto GetIdStrStatic() -> std::string\
{\
    return id_str;\
}\
[[nodiscard]]\
virtual auto GetIdStr() const -> std::string override\
{\
    return GetIdStrStatic();\
}

namespace Arcadia
{
    struct ARCADIA_API iImguiWindow
    {
    public:
        using self_type = iImguiWindow;
    public:
        iImguiWindow(
            bool open = false,
            const std::string& title={}
        ):
            _Open(open),
            _Title(title)
        {}
        virtual ~iImguiWindow() = default;

        [[nodiscard]]
        auto IsOpen() const -> bool
        {
            return _Open;
        }

        [[nodiscard]]
        auto GetTitle() const -> const std::string&
        {
            return _Title;
        }

        virtual void OnEvent(Arcadia::EventBase& event)
        {}
        virtual void OnUpdate() = 0;
        virtual auto GetIdStr() const->std::string = 0;

    protected:
        bool _Open;
        std::string _Title{};
    };

    template<class ImGuiWindow>
    concept cImguiWindow = requires{
        std::derived_from<ImGuiWindow, Arcadia::iImguiWindow>;
        {
            ImGuiWindow::GetIdStrStatic()
        } -> std::same_as<std::string>;
    };

}
