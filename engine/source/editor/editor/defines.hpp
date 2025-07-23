#pragma once

#include "platform/platform_detection.hpp"

#ifdef ACDA_EDITOR_EXPORT
#    ifdef ACDA_EDITOR
#        define ACDA_API ACDA_EXPORT_ATTRIBUTE
#    else
#        define ACDA_API ACDA_IMPORT_ATTRIBUTE
#    endif // ACDA_EDITOR
#else
#    define ACDA_API
#endif // ACDA_EDITOR_EXPORT