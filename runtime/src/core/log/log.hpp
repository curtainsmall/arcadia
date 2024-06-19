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

    static auto instance() -> self_type&;

    void debug(
        const std::string& msg
    )
    {
        _Logger->debug(msg);
    }

    void info(
        const std::string& msg
    )
    {
        _Logger->info(msg);
    }

    void warning(
        const std::string& msg
    )
    {
        _Logger->warn(msg);
    }

    void error(
        const std::string& msg
    )
    {
        _Logger->error(msg);
    }

    void fatal(
        const std::string& msg
    )
    {
        _Logger->critical(msg);
    }

    void demarcate(
        const std::string& msg
    )
    {
        debug(std::format("//==== {} ====//", msg));
    }

    void flush()
    {
        _Logger->flush();
    }

private:
    std::shared_ptr<spdlog::logger> _Logger{};
};

ACDA_API static inline void log_debug(
    const std::string& msg
)
{
    Logger::instance().debug(msg);
}

ACDA_API static inline void log_info(
    const std::string& msg
)
{
    Logger::instance().info(msg);
}

ACDA_API static inline void log_warning(
    const std::string& msg
)
{
    Logger::instance().warning(msg);
}

ACDA_API static inline void log_error(
    const std::string& msg
)
{
    Logger::instance().error(msg);
}

ACDA_API static inline void log_fatal(
    const std::string& msg
)
{
    Logger::instance().fatal(msg);
}

ACDA_API static inline void log_demarcate(
    const std::string& msg
)
{
    log_debug(std::format("//==== {} ====//", msg));
}

ACDA_API static inline void log_flush()
{
    Logger::instance().flush();
}
