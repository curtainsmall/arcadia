#pragma once

#include<cassert>
#include<cstddef>
#include<cstdint>
#include<cstdlib>
#include<typeindex>

#include"platform/base.hpp"

#define ARCADIA_ASSERT(x) assert(x)

namespace arcadia
{
    struct ARCADIA_API noncopyable
    {
    protected:
        noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        auto operator=(const noncopyable&) = delete;
    };

    template<class ...Args>
    ARCADIA_API void discard(Args&& ...)
    {}

}
