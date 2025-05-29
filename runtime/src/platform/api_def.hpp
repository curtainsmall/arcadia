#pragma once

#ifdef _WIN32
#   include<Windows.h>
#   ifdef ACDA_DYNAMIC_LINK
#       ifdef ACDA_EXPORT
#           define ACDA_API __declspec(dllexport)
#       else
#           define ACDA_API __declspec(dllimport)
#       endif // ACDA_EXPORT
#   else
#       define ACDA_API
#   endif // ACDA_DYNAMIC_LINK
#else
#   error Arcadia Engine does not support your operating system
#endif // _WIN32