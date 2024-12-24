#pragma once

#include<exception>
#include<string>

#include"platform/api_def.hpp"

#define _ACDA_GET_DEFINE_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_EXCEPTION(...) \
_ACDA_GET_DEFINE_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_EXCEPTION)(__VA_ARGS__)

#define _ACDA_DEFINE_EXCEPTION(exception_name) \
class exception_name: public ::Arcadia::Exception{\
public:\
        inline exception_name(const std::string& msg = #exception_name) :\
        ::Arcadia::Exception(msg)\
    {}\
}

#define _ACDA_DEFINE_EXCEPTION_WITH_MESSAGE(exception_name, message) \
class exception_name: public ::Arcadia::Exception{\
public:\
    inline exception_name(const std::string& msg = message ):\
        ::Arcadia::Exception(msg)\
    {}\
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

    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(NullReturn);
    }
}