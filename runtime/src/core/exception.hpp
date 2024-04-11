#pragma once

#include<exception>
#include<string>

#include"core/base.hpp"

#define ARCADIA_EXCEPTION(exception_name) \
struct ARCADIA_API exception_name: Arcadia::Exception{\
inline exception_name(const std::string& msg = #exception_name ):\
Arcadia::Exception(msg){\
}\
}

namespace Arcadia
{
    struct ARCADIA_API Exception: std::exception
    {
    public:
        Exception():
            std::exception()
        {}

        explicit Exception(const std::string& msg):
            std::exception(msg.c_str())
        {}
    };

}
