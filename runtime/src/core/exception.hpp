#pragma once

#include<exception>
#include<string>

#include"core/base.hpp"

#define ARCADIA_EXCEPTION(exception_name) \
struct exception_name: Exception{\
inline exception_name(const std::string& msg = #exception_name ):\
Exception(msg){\
}\
}

struct Exception: std::exception
{
public:
    Exception():
        std::exception()
    {}

    explicit Exception(const std::string& msg):
        std::exception(msg.c_str())
    {}
};
