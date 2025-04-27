#pragma once

#include<exception>
#include<string>

#include"platform/api_def.hpp"
#include"core/log/log.hpp"

namespace Arcadia::Exceptions
{
    class BasicException: public std::exception
    {
    public:
        explicit BasicException(const std::string& msg = "Basic Exception"):
            std::exception(msg.c_str())
        {}

        virtual ~BasicException() = default;

        auto GetErrorMessage() const -> const char*
        {
            return what();
        }
    };

    class RuntimeError: public BasicException
    {
    public:
        explicit RuntimeError(const std::string& msg = "(No Message Given)"):
            BasicException(msg)
        {
            ACDA_LOG_ERROR(std::format("Runtime Exception: {}", msg));
        }
    };

    class LogicError: public BasicException
    {
    public:
        explicit LogicError(const std::string& msg = "(No Message Given)"):
            BasicException(msg)
        {
            ACDA_LOG_ERROR(std::format("Logic Exception {}", msg));
        }
    };
}

//==== Logic error exception ====

#define _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION(exception_name) \
class exception_name: public Arcadia::Exceptions::LogicError{\
public:\
        inline exception_name(const std::string& msg = #exception_name) :\
        Arcadia::Exceptions::LogicError(msg)\
    {}\
}

#define _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION_WITH_MESSAGE(exception_name, message) \
class exception_name: public Arcadia::Exceptions::LogicError{\
public:\
    inline exception_name(const std::string& msg = message ):\
        Arcadia::Exceptions::LogicError(msg)\
    {}\
}

#define _ACDA_GET_DEFINE_LOGIC_ERROR_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_LOGIC_ERROR_EXCEPTION(...) \
_ACDA_GET_DEFINE_LOGIC_ERROR_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION)(__VA_ARGS__)

//==== Runtime error exception ====

#define _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(exception_name) \
class exception_name: public Arcadia::Exceptions::RuntimeError{\
public:\
        inline exception_name(const std::string& msg = #exception_name) :\
        Arcadia::Exceptions::RuntimeError(msg)\
    {}\
}

#define _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION_WITH_MESSAGE(exception_name, message) \
class exception_name: public Arcadia::Exceptions::RuntimeError{\
public:\
    inline exception_name(const std::string& msg = message ):\
        Arcadia::Exceptions::RuntimeError(msg)\
    {}\
}

#define _ACDA_GET_DEFINE_RUNTIME_ERROR_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(...) \
_ACDA_GET_DEFINE_RUNTIME_ERROR_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION)(__VA_ARGS__)

#define _ACDA_EXCPETION_MESSAGE_WRAPPER(msg) std::format("{} at {}:{}",msg,__FILE__, __LINE__)
#define _ACDA_THROW(exception_name) throw exception_name(_ACDA_EXCPETION_MESSAGE_WRAPPER("(No Exception Message)"))
#define _ACDA_THROW_WITH_MESSAGE(exception_name, msg) throw exception_name(_ACDA_EXCPETION_MESSAGE_WRAPPER(msg))
#define _ACDA_GET_THROW_MACRO(_1,_2, name, ...) name
#define ACDA_THROW(...) _ACDA_GET_THROW_MACRO(__VA_ARGS__, _ACDA_THROW_WITH_MESSAGE,_ACDA_THROW)(__VA_ARGS__)

