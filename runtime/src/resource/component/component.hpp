#pragma once

#include"core/base.hpp"

namespace arcadia
{
    struct ARCADIA_API component
    {};

    template<class Compont>
    concept component_like = requires{
        std::derived_from<Compont, arcadia::component>;
    };
}
