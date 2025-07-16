#include "imgui_window_console.hpp"

Arcadia::ImguiWindowConsole::ImguiWindowConsole(bool open, const std::string& title):
    ImguiWindowInterface(open, title),
    _ImguiConsole(title)
{
}

void Arcadia::ImguiWindowConsole::OnEvent(EventBase& e)
{
}

void Arcadia::ImguiWindowConsole::OnUpdate()
{
    _ImguiConsole.Draw();
}
