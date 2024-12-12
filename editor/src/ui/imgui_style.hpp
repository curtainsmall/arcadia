#pragma once

#include"core/base.hpp"
#include"ui/imgui_header.hpp"

namespace Arcadia
{
    namespace ImguiStyle
    {
        ACDA_API void SetToDefaultDark();
        ACDA_API void SetToDefaultClassic();
        ACDA_API void SetToDefaultLight();

        /// @brief From https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
        ACDA_API void SetToEmbraceDarkness();

        /// @brief From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp#L116
        ACDA_API void SetToDark();
    }
}