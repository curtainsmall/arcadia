#pragma once

#include <functional>

namespace Arcadia
{
    // This function including constructions, main loop and destructions
    // This function is seperated in case you want to declare your own main function while executing everything as normal
    // The "setup_function" is used to setup the application (either editor or game) and is called after the RuntimeLayer is constructed
    ACDA_API auto Main(const std::function<void()>& setup_funcion = {}) -> int;
}

