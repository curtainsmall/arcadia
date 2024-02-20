#pragma once

#include<exception>
#include<string>

#include"core/base.hpp"

#define ARCADIA_EXCEPTION(exception_name) \
struct ARCADIA_API exception_name: arcadia::exception{\
inline exception_name(const std::string& msg = #exception_name ):\
arcadia::exception(msg){\
}\
}

namespace arcadia
{
    struct ARCADIA_API exception: std::exception
    {
    public:
        inline exception():
            std::exception()
        {}

        inline explicit exception(const std::string& msg):
            std::exception(msg.c_str())
        {}
    };

}
