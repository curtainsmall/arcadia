#pragma once

#include<exception>
#include<string>

#include"platform/api_def.hpp"

#define ACDA_DEFINE_EXCEPTION(exception_name) \
class exception_name: public ::Arcadia::Exception{\
public:\
    inline exception_name(const std::string& msg = #exception_name ):\
        ::Arcadia::Exception(msg){\
    }\
}

namespace Arcadia
{
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

    ACDA_DEFINE_EXCEPTION(NullReturn);
}