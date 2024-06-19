#pragma once

#include"core/base.hpp"
#include"ui/imgui_header.hpp"

namespace imgui_style
{
    ACDA_API void default_dark();
    ACDA_API void default_classic();
    ACDA_API void default_light();

    /// @brief From https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
    ACDA_API void embrace_darkness();

    /// @brief From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp#L116
    ACDA_API void dark();

}

