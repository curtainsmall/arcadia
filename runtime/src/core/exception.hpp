#pragma once

#include<exception>
#include<string>

#include"platform/api_def.hpp"

//==== Logic error exception ====

#define _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION(exception_name) \
class exception_name: public ::std::logic_error{\
public:\
        inline exception_name(const std::string& msg = #exception_name) :\
        ::std::logic_error(msg)\
    {}\
}

#define _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION_WITH_MESSAGE(exception_name, message) \
class exception_name: public ::std::logic_error{\
public:\
    inline exception_name(const std::string& msg = message ):\
        ::std::logic_error(msg)\
    {}\
}

#define _ACDA_GET_DEFINE_LOGIC_ERROR_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_LOGIC_ERROR_EXCEPTION(...) \
_ACDA_GET_DEFINE_LOGIC_ERROR_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_LOGIC_ERROR_EXCEPTION)(__VA_ARGS__)

//==== Runtime error exception ====

#define _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(exception_name) \
class exception_name: public ::std::runtime_error{\
public:\
        inline exception_name(const std::string& msg = #exception_name) :\
        ::std::runtime_error(msg)\
    {}\
}

#define _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION_WITH_MESSAGE(exception_name, message) \
class exception_name: public ::std::runtime_error{\
public:\
    inline exception_name(const std::string& msg = message ):\
        ::std::runtime_error(msg)\
    {}\
}

#define _ACDA_GET_DEFINE_RUNTIME_ERROR_EXCEPTION_MACRO(_1, _2, name, ...) name

#define ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION(...) \
_ACDA_GET_DEFINE_RUNTIME_ERROR_EXCEPTION_MACRO(__VA_ARGS__, _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION_WITH_MESSAGE, _ACDA_DEFINE_RUNTIME_ERROR_EXCEPTION)(__VA_ARGS__)