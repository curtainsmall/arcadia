#pragma once

namespace Arcadia
{
    class Noncopyable
    {
    protected:
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        auto operator=(const Noncopyable&) = delete;
    };
}