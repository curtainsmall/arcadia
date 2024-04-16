#include "pch.hpp"
#include "log.hpp"

#include"spdlog/async.h"
#include"spdlog/sinks/basic_file_sink.h"
#include"spdlog/sinks/stdout_color_sinks.h"

Arcadia::Logger::Logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("%^[%x %X]%$ %v");

    const spdlog::sinks_init_list sink_list ={ console_sink };

    spdlog::init_thread_pool(8192, 1);

    _Logger = std::make_shared<spdlog::async_logger>(
        "main",
        sink_list.begin(),
        sink_list.end(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );

    _Logger->set_level(spdlog::level::trace);
    _Logger->flush_on(spdlog::level::debug);
    spdlog::register_logger(_Logger);
}

Arcadia::Logger::~Logger()
{
    _Logger->flush();
    spdlog::drop_all();
    spdlog::shutdown();
}

auto Arcadia::Logger::Instance() -> self_type&
{
    static self_type logger{};
    return logger;
}
