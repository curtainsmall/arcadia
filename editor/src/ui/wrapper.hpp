#pragma once

#include<string>

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API imgui_wrapper
    {
    public:
        imgui_wrapper(
            const std::string& title,
            bool open = false
        );
        virtual ~imgui_wrapper() = default;

        virtual void on_render() = 0;

    public:
        bool open;
        std::string title;
    };
}
