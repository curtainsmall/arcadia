#pragma once

#include <functional>

#if defined(_WIN32) && !defined(ACDA_DEBUG_MODE)
#define ACDA_MAIN_FN_DECL int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
#define ACDA_MAIN_FN_DECL int main()
#endif // _WIN32

namespace Arcadia
{
    // This function including constructions, main loop and destructions
    // This function is seperated in case you want to declare your own main function while executing everything as normal
    // The "application_setup_function" is called after the RuntimeLayer is constructed
    ACDA_API auto Main(const std::function<void()>& application_setup_funcion = {}) -> int;
}

extern auto Arcadia::Main(const std::function<void()>& application_setup_funcion) -> int;

#define ACDA_MAIN_FN(app_setup_fn) ACDA_MAIN_FN_DECL{ return Arcadia::Main(app_setup_fn);}
