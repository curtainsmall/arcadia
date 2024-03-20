#pragma once

#ifdef _WIN32

#ifdef ARCADIA_DYNAMIC_LIB

#ifdef ARCADIA_EXPORTS
#define ARCADIA_API __declspec(dllexport)
#else
#define ARCADIA_API __declspec(dllimport)
#endif // ARCADIA_EXPORTS

#else
#define ARCADIA_API
#endif // ARCADIA_DYNAMIC_LIB

#else 
#error Arcadia Engine does not support your operating system
#endif // _WIN32

#define ARCADIA_DEBUG 1
#if !defined(NDEBUG) && ARCADIA_DEBUG
#   define ARCADIA_IN_DEBUG
#endif



