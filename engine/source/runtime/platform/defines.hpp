#pragma once

// Import/Export
#ifdef _WIN32
#   include<Windows.h>
#   ifdef ACDA_DYNAMIC_RUNTIME_LIB
#       ifdef ACDA_EXPORT
#           define ACDA_API __declspec(dllexport)
#       else
#           define ACDA_API __declspec(dllimport)
#       endif // ACDA_EXPORT
#   else
#       define ACDA_API
#   endif // ACDA_DYNAMIC_RUNTIME_LIB
#else
#   error Arcadia Engine does not support your operating system
#endif // _WIN32

// Debug mode
#define ACDA_DEBUG 1
#if !defined(NDEBUG) && ACDA_DEBUG
#   define ACDA_DEBUG_MODE
#endif //!defined(NDEBUG) && ACDA_DEBUG

// Default working directory
#ifndef ACDA_DEFAULT_WORKING_DIR
#   define ACDA_DEFAULT_WORKING_DIR "./"
#endif // !ACDA_DEFAULT_WORKING_DIR



