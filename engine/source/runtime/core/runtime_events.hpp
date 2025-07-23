#pragma once

#include "platform/defines.hpp"
#include "core/event.hpp"

namespace Arcadia::Events
{
    struct ACDA_API RuntimeStart: public EventBase
    {
    };

    struct ACDA_API RuntimeStop: public EventBase
    {
    };
}