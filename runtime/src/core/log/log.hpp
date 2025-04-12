#pragma once

#include<memory>
#include<string>


#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include"spdlog/spdlog.h"

#include"platform/api_def.hpp"
#include"platform/debug_def.hpp"

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
            DumpBacktrace();
        }

        void LogAsFatal(const std::string& msg)
        {
            _Logger->critical(msg);
            DumpBacktrace();
        }

        void LogDemarcateLine(const std::string& msg)
        {
            LogAsDebug(std::format("//==== {} ====//", msg));
        }

        void Flush()
        {
            _Logger->flush();
        }

        void EnableBacktrace(std::size_t count)
        {
            _Logger->enable_backtrace(count);
        }

        void DisableBacktrace()
        {
            _Logger->disable_backtrace();
        }

        void DumpBacktrace()
        {
            _Logger->dump_backtrace();
        }

    private:
        std::shared_ptr<spdlog::logger> _Logger{};
    };
}

#define ACDA_LOG_DEBUG(msg)     (::Arcadia::Logger::Instance().LogAsDebug(msg))
#define ACDA_LOG_INFO(msg)      (::Arcadia::Logger::Instance().LogAsInfo(msg))
#define ACDA_LOG_WARNING(msg)   (::Arcadia::Logger::Instance().LogAsWarning(msg))
#define ACDA_LOG_ERROR(msg)     (::Arcadia::Logger::Instance().LogAsError(msg))
#define ACDA_LOG_FATAL(msg)     (::Arcadia::Logger::Instance().LogAsFatal(msg))
#define ACDA_LOG_DEMARCATE(msg) ACDA_LOG_DEBUG(std::format("//==== {} ====//", msg))
#define ACDA_LOG_FLUSH()        (::Arcadia::Logger::Instance().Flush())
