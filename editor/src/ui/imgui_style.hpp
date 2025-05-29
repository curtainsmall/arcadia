#pragma once

#include "platform/api_def.hpp"
#include "ui/imgui.hpp"

namespace Arcadia
{
    namespace ImguiStyle
    {
         void SetToDefaultDark();
         void SetToDefaultClassic();
         void SetToDefaultLight();

        // From https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
         void SetToEmbraceDarkness();

        // From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp#L116
         void SetToDark();
    }
}