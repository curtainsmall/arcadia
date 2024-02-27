#pragma once

#include"core/base.hpp"
#include"core/event/event.hpp"

namespace arcadia
{
    namespace event
    {
        ARCADIA_EVENT(
            monitor_connection,
            GLFWmonitor*, // TODO: Use custom monitor type
            bool // Whether the monitor is connected or not
        );
    }
}
