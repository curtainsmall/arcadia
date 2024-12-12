#pragma once

#include<memory>
#include<string>

#include"spdlog/spdlog.h"

#include"core/base.hpp"

namespace Arcadia
{
    class Logger
    {
    public:
        using SelfType = Logger;
    public:
        Logger();
        ~Logger();

        static auto Instance() -> SelfType&;

        void LogAsDebug(const std::string& msg)
        {
            _Logger->debug(msg);
        }

        void LogAsInfo(const std::string& msg)
        {
            _Logger->info(msg);
        }

        void LogAsWarning(const std::string& msg)
        {
            _Logger->warn(msg);
        }

        void LogAsError(const std::string& msg)
        {
            _Logger->error(msg);
        }

        void LogAsFatal(const std::string& msg)
        {
            _Logger->critical(msg);
        }

        void LogDemarcateLine(const std::string& msg)
        {
            LogAsDebug(std::format("//==== {} ====//", msg));
        }

        void Flush()
        {
            _Logger->flush();
        }

    private:
        std::shared_ptr<spdlog::logger> _Logger{};
    };
}

#ifdef ACDA_DEBUG_MODE
#   define ACDA_LOG_DEBUG(msg)     Arcadia::Logger::Instance().LogAsDebug(msg)
#   define ACDA_LOG_INFO(msg)      Arcadia::Logger::Instance().LogAsInfo(msg)
#   define ACDA_LOG_WARNING(msg)   Arcadia::Logger::Instance().LogAsWarning(msg)
#   define ACDA_LOG_ERROR(msg)     Arcadia::Logger::Instance().LogAsError(msg)
#   define ACDA_LOG_FATAL(msg)     Arcadia::Logger::Instance().LogAsFatal(msg)
#   define ACDA_LOG_DEMARCATE(msg) ACDA_LOG_DEBUG(std::format("//==== {} ====//", msg))
#   define ACDA_LOG_FLUSH()        Arcadia::Logger::Instance().Flush()
#else
#   define ACDA_LOG_DEBUG(msg)
#   define ACDA_LOG_INFO(msg)
#   define ACDA_LOG_WARNING(msg)
#   define ACDA_LOG_ERROR(msg)
#   define ACDA_LOG_FATAL(msg)
#   define ACDA_LOG_DEMARCATE(msg)
#   define ACDA_LOG_FLUSH()
#endif