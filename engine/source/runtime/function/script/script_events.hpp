#pragma once

#include <string_view>

#include "core/event.hpp"
#include "platform/defines.hpp"

namespace Arcadia::Events
{
    struct ACDA_API ExecuteScript: public EventBase
    {
    public:
        ExecuteScript(std::string_view script_text):
            ScriptText(script_text)
        {
        }
    public:
        std::string_view ScriptText;
    };
}