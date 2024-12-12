#pragma once

#include<exception>
#include<string>

#include"core/base.hpp"

#define ACDA_EXCEPTION(exception_name) \
class exception_name: public Exception{\
public:\
inline exception_name(const std::string& msg = #exception_name ):\
Exception(msg){\
}\
}

class Exception: public std::exception
{
public:
    Exception() :
        std::exception()
    {}

    explicit Exception(const std::string& msg) :
        std::exception(msg.c_str())
    {}
};