#pragma once

#include<memory>
#include<string>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"function/ui/imgui_window.hpp"

#include"project/project.hpp"
#include"project/project_events.hpp"

namespace arcadia
{

    struct ARCADIA_API imgui_window_menubar: arcadia::imgui_window_interface
    {
    public:
        using self_type = imgui_window_menubar;
    public:
        ARCADIA_IMGUI_WINDOW_ID_STR_GETERS("###menubar");

        inline imgui_window_menubar(bool open, const std::initializer_list<std::tuple<std::string, std::string>>& imgui_window_title_id_pairs):
            arcadia::imgui_window_interface(open, "Menubar"),
            _imgui_window_title_and_id_str_pairs(imgui_window_title_id_pairs)
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
        std::weak_ptr<const arcadia::project> _project_wptr{};
        std::vector<std::tuple<std::string, std::string>> _imgui_window_title_and_id_str_pairs{};
    };
}