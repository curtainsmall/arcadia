#pragma once

#include<memory>
#include<string>

#include"spdlog/spdlog.h"

#include"core/base.hpp"

struct Logger
{
public:
    using self_type = Logger;
public:
    Logger();
    ~Logger();

    static auto Instance() -> self_type&;

    void Debug(
        const std::string& msg
    )
    {
        _Logger->debug(msg);
    }

    void Info(
        const std::string& msg
    )
    {
        _Logger->info(msg);
    }

    void Warning(
        const std::string& msg
    )
    {
        _Logger->warn(msg);
    }

    void Error(
        const std::string& msg
    )
    {
        _Logger->error(msg);
    }

    void Fatal(
        const std::string& msg
    )
    {
        _Logger->critical(msg);
    }

    void Demarcate(
        const std::string& msg
    )
    {
        Debug(std::format("//==== {} ====//", msg));
    }

    void Flush()
    {
        _Logger->flush();
    }

private:
    std::shared_ptr<spdlog::logger> _Logger{};
};

namespace Log
{
    ARCADIA_API static inline void Debug(
        const std::string& msg
    )
    {
        Logger::Instance().Debug(msg);
    }

    ARCADIA_API static inline void Info(
        const std::string& msg
    )
    {
        Logger::Instance().Info(msg);
    }

    ARCADIA_API static inline void Warning(
        const std::string& msg
    )
    {
        Logger::Instance().Warning(msg);
    }

    ARCADIA_API static inline void Error(
        const std::string& msg
    )
    {
        Logger::Instance().Error(msg);
    }

    ARCADIA_API static inline void Fatal(
        const std::string& msg
    )
    {
        Logger::Instance().Fatal(msg);
    }

    ARCADIA_API static inline void Demarcate(
        const std::string& msg
    )
    {
        Debug(std::format("//==== {} ====//", msg));
    }

    ARCADIA_API static inline void Flush()
    {
        Logger::Instance().Flush();
    }
}
