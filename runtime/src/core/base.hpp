#pragma once

#include<cassert>
#include<chrono>
#include<complex>
#include<cstddef>
#include<cstdint>
#include<cstdlib>
#include<string>
#include<typeindex>
#include<typeinfo>

#include"platform/base.hpp"

#define ARCADIA_DEBUG 1
#if !defined(NDEBUG) && ARCADIA_DEBUG
#   define ARCADIA_IN_DEBUG
#endif

#define ARCADIA_ASSERT(x) assert(x)

#define ARCADIA_BIND_MEMBER_FN(fn) [this]<class ...Args>(Args&& ...args) -> decltype(auto) { return this->fn(std::forward<Args>(args)...); }

#define ARCADIA_DISCARD(x) (void) x

using namespace std::string_literals;
using namespace std::string_view_literals;
using namespace std::chrono_literals;
using namespace std::complex_literals;

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
