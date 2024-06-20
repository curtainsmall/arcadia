#pragma once

#ifdef _WIN32
#   include<Windows.h>
#   ifdef ACDA_DYNAMIC_LIB
#       ifdef ACDA_EXPORTS
#           define ACDA_API __declspec(dllexport)
#       else
#           define ACDA_API __declspec(dllimport)
#       endif // ACDA_EXPORTS
#   else
#       define ACDA_API
#   endif // ACDA_DYNAMIC_LIB
#else 
#   error Arcadia Engine does not support your operating system
#endif // _WIN32

#define ACDA_DEBUG 1
#if !defined(NDEBUG) && ACDA_DEBUG
#   define ACDA_DEBUG_MODE
#endif



