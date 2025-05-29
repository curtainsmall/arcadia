#pragma once

namespace Arcadia
{
    struct ACDA_API Noncopyable
    {
    protected:
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        auto operator=(const Noncopyable&) = delete;
    };
}