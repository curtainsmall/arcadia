#pragma once

#include "platform/platform_detection.hpp"

// Import/Export
#ifdef ACDA_RUNTIME_EXPORT
#    ifdef ACDA_RUNTIME
#        define ACDA_API ACDA_EXPORT_ATTRIBUTE
#    else
#        define ACDA_API ACDA_IMPORT_ATTRIBUTE
#    endif // ACDA_RUNTIME
#else
#    define ACDA_API
#endif // ACDA_RUNTIME_EXPORT

// Debug mode
#define ACDA_DEBUG 1
#if !defined(NDEBUG) && ACDA_DEBUG
#   define ACDA_DEBUG_MODE
#endif //!defined(NDEBUG) && ACDA_DEBUG

// Default working directory
#ifndef ACDA_DEFAULT_WORKING_DIR
#   define ACDA_DEFAULT_WORKING_DIR "./"
#endif // !ACDA_DEFAULT_WORKING_DIR