#include "wrapper.hpp"

arcadia::imgui_wrapper::imgui_wrapper(const std::string& title, bool open):
    title(title),
    open(open)
{}

arcadia::imgui_wrapper::~imgui_wrapper() = default;
