#pragma once

namespace Arcadia
{
    struct Noncopyable
    {
    protected:
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        auto operator=(const Noncopyable&) = delete;
    };
}