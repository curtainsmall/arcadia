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

    void debug(const std::string& msg)
    {
        _Logger->debug(msg);
    }

    void info(const std::string& msg)
    {
        _Logger->info(msg);
    }

    void warning(const std::string& msg)
    {
        _Logger->warn(msg);
    }

    void error(const std::string& msg)
    {
        _Logger->error(msg);
    }

    void fatal(const std::string& msg)
    {
        _Logger->critical(msg);
    }

    void demarcate(const std::string& msg)
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

#ifdef ACDA_DEBUG_MODE
#   define ACDA_LOG_DEBUG(msg)     Logger::instance().debug(msg)
#   define ACDA_LOG_INFO(msg)      Logger::instance().info(msg)
#   define ACDA_LOG_WARNING(msg)   Logger::instance().warning(msg)
#   define ACDA_LOG_ERROR(msg)     Logger::instance().error(msg)
#   define ACDA_LOG_FATAL(msg)     Logger::instance().fatal(msg)
#   define ACDA_LOG_DEMARCATE(msg) ACDA_LOG_DEBUG(std::format("//==== {} ====//", msg))
#   define ACDA_LOG_FLUSH()        Logger::instance().flush()
#else
#   define ACDA_LOG_DEBUG(msg)     
#   define ACDA_LOG_INFO(msg)      
#   define ACDA_LOG_WARNING(msg)   
#   define ACDA_LOG_ERROR(msg)     
#   define ACDA_LOG_FATAL(msg)     
#   define ACDA_LOG_DEMARCATE(msg) 
#   define ACDA_LOG_FLUSH()        
#endif
