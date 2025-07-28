#pragma once

#include <exception>
#include <string>

#include "core/log.hpp"
#include "platform/defines.hpp"

namespace Arcadia::Exceptions
{
    struct ACDA_API BasicException: public std::exception
    {
    public:
        explicit BasicException(std::string_view msg = "Basic Exception"):
            std::exception(msg.data())
        {
        }

        virtual ~BasicException() = default;

        auto GetErrorMessage() const -> const char*
        {
            return what();
        }
    };
}

#define _ACDA_DEFINE_EXCEPTION(exception_name) \
struct ACDA_API exception_name: public Arcadia::Exceptions::BasicException{\
public:\
        inline exception_name(std::string_view msg = #exception_name) :\
        Arcadia::Exceptions::BasicException(msg)\
    {}\
}

#define _ACDA_DEFINE_EXCEPTION_WITH_MESSAGE(exception_name, message) \
struct ACDA_API exception_name: public Arcadia::Exceptions::BasicException{\
public:\
    inline exception_name(std::string_view msg = message ):\
        Arcadia::Exceptions::BasicException(msg)\
    {}\
}

#define _ACDA_GET_DEFINE_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_EXCEPTION(...) \
_ACDA_GET_DEFINE_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_EXCEPTION)(__VA_ARGS__)

#define _ACDA_EXCPETION_MESSAGE_WRAPPER(msg) std::format("{} at {}:{}",msg,__FILE__, __LINE__)
#define _ACDA_THROW(exception_name) throw exception_name(_ACDA_EXCPETION_MESSAGE_WRAPPER("(No Exception Message)"))
#define _ACDA_THROW_WITH_MESSAGE(exception_name, msg) throw exception_name(_ACDA_EXCPETION_MESSAGE_WRAPPER(msg))
#define _ACDA_GET_THROW_MACRO(_1,_2, name, ...) name
#define ACDA_THROW(...) _ACDA_GET_THROW_MACRO(__VA_ARGS__, _ACDA_THROW_WITH_MESSAGE,_ACDA_THROW)(__VA_ARGS__)