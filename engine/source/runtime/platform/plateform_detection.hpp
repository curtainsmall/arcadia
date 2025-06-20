#pragma once

#ifdef _WIN32 // Windows x86/x64
#   ifdef _WIN64
#       define ACDA_PLATFORM_WINDOWS
#   else
#       error "x86 builds are not supported"
#   endif // _WIN64
#elif defined(__APPLE__) || defined(__MACH__) // Apple
#   include <TargetConditional.h>
// TARGET_OS_MAC exsits on all the platforms, so we must check all of them(in this order)
// to ensure that we are running on MAC and not some other Apple platform
#   if TARGET_IPHONE_SIMULATOR == 1
#       error "IOS simulator is not supported"
#   elif TRAGET_OS_IPHONE == 1
#       define ACDA_PLATFORM_IOS
#       error "IOS is not supported"
#   elif TARGET_OS_MAC == 1
#       define ACDA_PLATFORM_MACOS
#       error "MacOS is not supported"
#   else
#       error "Unknown Apple platform"
#   endif
#elif defined(__ANDROID__) // Android
#   define ACDA_PLATFORM_ANDROID
#   error "Android is not supported"
#elif defined(__linux__) // Linux
#   define ACDA_PLATFORM_LINUX
#   error "Linux is not supported"
#else
#   error "Unknown platform"
#endif