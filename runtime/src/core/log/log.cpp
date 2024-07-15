#include "pch.hpp"

#include "log.hpp"

#include"spdlog/async.h"
#include"spdlog/sinks/basic_file_sink.h"
#include"spdlog/sinks/stdout_color_sinks.h"

Logger::Logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("%^[%x %X]%$ %v");

    const spdlog::sinks_init_list sink_list ={ console_sink };

    spdlog::init_thread_pool(8192, 1);

    _logger = std::make_shared<spdlog::async_logger>(
        "main",
        sink_list.begin(),
        sink_list.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    _logger->set_level(spdlog::level::trace);
    _logger->flush_on(spdlog::level::debug);
    spdlog::register_logger(_logger);
}

Logger::~Logger()
{
    _logger->flush();
    spdlog::drop_all();
    spdlog::shutdown();
}

auto Logger::instance() -> self_type&
{
    static self_type logger{};
    return logger;
}
