#pragma once

#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"


#include"project/project.hpp"
#include"project/project_events.hpp"
#include"ui/imgui_windows/imgui_window.hpp"

namespace arcadia
{

    struct ARCADIA_API imgui_window_menubar: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_menubar;
    public:
        [[nodiscard]]
        static constexpr auto get_title() -> std::string
        {
            return "Menu Bar";
        }

        inline imgui_window_menubar(bool open, const std::initializer_list<std::string>& imgui_window_titles):
            arcadia::imgui_window_interface(open),
            _imgui_window_titles(imgui_window_titles)
        {}
        virtual ~imgui_window_menubar() = default;

        virtual void on_event(arcadia::event_base& event) override;
        virtual void on_update() override;


    private:
        void _file_menu();
        void _edit_menu();
        void _view_menu();

        void _on_project_built(arcadia::event::project_built& e);
        void _on_project_unbuilt(arcadia::event::project_unbuilt& e);
    private:
        const arcadia::project* _project_cptr{};
        std::vector<std::string> _imgui_window_titles{};
    };
}