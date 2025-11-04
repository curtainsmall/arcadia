#include "imgui_window_console.hpp"

#include "core/function.hpp"

#include "editor/editor_layer.hpp"
#include "ui/imgui_wrapper.hpp"

Arcadia::ImguiWindowConsole::ImguiWindowConsole(bool open, std::string_view title):
    ImguiWindowInterface(open, title)
{
    _ColorPalette[ToUnderlying(ColorPalette::Command)] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _ColorPalette[ToUnderlying(ColorPalette::Log)] = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    _ColorPalette[ToUnderlying(ColorPalette::Warning)] = glm::vec4(1.0f, 0.87f, 0.37f, 1.0f);
    _ColorPalette[ToUnderlying(ColorPalette::Error)] = glm::vec4(1.0f, 0.365f, 0.365f, 1.0f);
    _ColorPalette[ToUnderlying(ColorPalette::Info)] = glm::vec4(0.46f, 0.96f, 0.46f, 1.0f);
    _ColorPalette[ToUnderlying(ColorPalette::Timestamp)] = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
}

void Arcadia::ImguiWindowConsole::OnEvent(EventBase& e)
{
    EventDispatcher(e)
        .Dispatch<Events::OpenImguiWindow>(ACDA_BIND_MEMBER_FN(_OnOpenImguiWindow))
        .IsDispatched();
}

void Arcadia::ImguiWindowConsole::OnUpdate()
{
    if(!_Opened)
    {
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
    if(ImGui::Begin(_Title.c_str(), &_Opened, window_flags))
    {
        ImGui::PopStyleVar();

        _MenuBar();

        _LogArea();

        ImGui::Separator();

        _InputBar();
    }
    else
    {
        ImGui::PopStyleVar();
    }
    ImGui::End();
    return;
}

void Arcadia::ImguiWindowConsole::_OnOpenImguiWindow(Events::OpenImguiWindow& e)
{
    if(e.IdString == GetIdString())
    {
        _Opened = true;
    }
}

void Arcadia::ImguiWindowConsole::_MenuBar()
{
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("Settings"))
        {
            ImGui::Checkbox("Colored Output", &_ColoredOutput);
            ImGui::SameLine();
            ImguiWrappers::HelpMark("Enable colored command output");

            ImGui::Checkbox("Auto scroll", &_AutoScroll);
            ImGui::SameLine();
            ImguiWrappers::HelpMark("Automatically scroll to the bottom of the display area");

            ImGui::Checkbox("Timestamp", &_Timestamp);
            ImGui::SameLine();
            ImguiWrappers::HelpMark("Display command execution timestamp");

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Arcadia::ImguiWindowConsole::_LogArea()
{
    const float footer_height_reserved = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if(ImGui::BeginChild("Display##", glm::vec2(0, -footer_height_reserved), ImGuiChildFlags_None, ImGuiWindowFlags_None))
    {
        static const float timestamp_width = ImGui::CalcTextSize("00:00:00:0000").x;
        std::size_t count = 0;

        ImGui::PushTextWrapPos();

        std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
        csys::System& system = editor_layer_sptr->GetCsysSystem();

        for(const auto& item : system.Items())
        {
            if(!_TextFilter.PassFilter(item.Get().c_str()))
            {
                continue;
            }

            if(item.m_Type == csys::COMMAND)
            {
                if(_Timestamp)
                {
                    ImGui::PushTextWrapPos(ImGui::GetColumnWidth());
                }
                if(count++ != 0)
                {
                    ImGui::Dummy(glm::vec2(-1, ImGui::GetFontSize()));
                }
            }

            if(_ColoredOutput)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, _ColorPalette[item.m_Type]);
                ImGui::TextUnformatted(item.Get().data());
                ImGui::PopStyleColor();
            }
            else
            {
                ImGui::TextUnformatted(item.Get().data());
            }

            if(item.m_Type == csys::COMMAND && _Timestamp)
            {
                ImGui::PopTextWrapPos();

                ImGui::SameLine(ImGui::GetColumnWidth(-1) - timestamp_width);

                ImGui::PushStyleColor(ImGuiCol_Text, _ColorPalette[ToUnderlying(ColorPalette::Timestamp)]);
                ImGui::Text(
                    "%02d:%02d:%02d:%04d",
                    (item.m_TimeStamp / 1000 / 3600) % 24,
                    (item.m_TimeStamp / 1000 / 60) % 60,
                    (item.m_TimeStamp / 1000) % 60,
                    item.m_TimeStamp % 1000
                );
                ImGui::PopStyleColor();
            }
        }

        ImGui::PopTextWrapPos();

        if(_ScrollToBottom &&
           (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() || _AutoScroll))
        {
            ImGui::SetScrollHereY(1.0f);
        }
        _ScrollToBottom = false;

        ImGui::EndChild();
    }
}

void Arcadia::ImguiWindowConsole::_InputBar()
{
    ImGuiInputTextFlags input_text_flags =
        ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_CallbackCompletion |
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways;

    bool reclaim_focus = false;

    std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
    csys::System& system = editor_layer_sptr->GetCsysSystem();

    ImGui::PushItemWidth(-ImGui::GetStyle().ItemSpacing.x * 7);
    if(ImGui::InputText(
        "Input",
        &_InputBuffer,
        input_text_flags,
        _InputCallback,
        this
    ))
    {
        if(!_InputBuffer.empty())
        {
            system.RunCommand(_InputBuffer);
            _ScrollToBottom = true;
        }

        reclaim_focus = true;

        _InputBuffer.clear();
    }

    ImGui::PopItemWidth();

    if(ImGui::IsItemEdited() && !_WasPrevFrameTabCompletion)
    {
        _CmdSuggestions.clear();
    }
    _WasPrevFrameTabCompletion = false;

    if(reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1);
}

auto Arcadia::ImguiWindowConsole::_InputCallback(ImGuiInputTextCallbackData* data) -> int
{
    // Exit if no buffer.
    if(data->BufTextLen == 0 && (data->EventFlag != ImGuiInputTextFlags_CallbackHistory))
        return 0;

    // Get input string and console.
    //std::string input_str = data->Buf;
    std::string trim_str;
    auto console = static_cast<ImguiWindowConsole*>(data->UserData);

    // Optimize by only using positions.
    // Trim start and end spaces.
    size_t start_pos = console->_InputBuffer.find_first_not_of(' ');
    size_t end_pos = console->_InputBuffer.find_last_not_of(' ');

    // Get trimmed string.
    if(start_pos != std::string::npos && end_pos != std::string::npos)
        trim_str = console->_InputBuffer.substr(start_pos, end_pos + 1);
    else
        trim_str = console->_InputBuffer;

    std::shared_ptr<EditorLayer> editor_layer_sptr = LayerStack::Instance().GetLayerShared<EditorLayer>();
    csys::System& system = editor_layer_sptr->GetCsysSystem();

    switch(data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Find last word.
            size_t start_substr_pos = trim_str.find_last_of(' ');
            csys::AutoComplete* console_autocomplete;

            // Command line is an entire word/string (No whitespace)
            // Determine which autocomplete tree to use.
            if(start_substr_pos == std::string::npos)
            {
                start_substr_pos = 0;
                console_autocomplete = &system.CmdAutocomplete();
            }
            else
            {
                start_substr_pos += 1;
                console_autocomplete = &system.VarAutocomplete();
            }

            // Validate str
            if(!trim_str.empty())
            {
                // Display suggestions on console.
                if(!console->_CmdSuggestions.empty())
                {
                    system.Log(csys::COMMAND) << "Suggestions: " << csys::endl;

                    for(const auto& suggestion : console->_CmdSuggestions)
                        system.Log(csys::LOG) << suggestion << csys::endl;

                    console->_CmdSuggestions.clear();
                }

                // Get partial completion and suggestions.
                std::string partial = console_autocomplete->Suggestions(trim_str.substr(start_substr_pos, end_pos + 1), console->_CmdSuggestions);

                // Autocomplete only when one work is available.
                if(!console->_CmdSuggestions.empty() && console->_CmdSuggestions.size() == 1)
                {
                    data->DeleteChars(static_cast<int>(start_substr_pos), static_cast<int>(data->BufTextLen - start_substr_pos));
                    data->InsertChars(static_cast<int>(start_substr_pos), console->_CmdSuggestions[0].data());
                    console->_CmdSuggestions.clear();
                }
                else
                {
                    // Partially complete word.
                    if(!partial.empty())
                    {
                        data->DeleteChars(static_cast<int>(start_substr_pos), static_cast<int>(data->BufTextLen - start_substr_pos));
                        data->InsertChars(static_cast<int>(start_substr_pos), partial.data());
                    }
                }
            }

            // We have performed the completion event.
            console->_WasPrevFrameTabCompletion = true;
        }
        break;

        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Clear buffer.
            data->DeleteChars(0, data->BufTextLen);

            // Init history index
            if(console->_HistoryIndex == std::numeric_limits<size_t>::min())
                console->_HistoryIndex = system.History().GetNewIndex();

            // Traverse history.
            if(data->EventKey == ImGuiKey_UpArrow)
            {
                if(console->_HistoryIndex) --(console->_HistoryIndex);
            }
            else
            {
                if(console->_HistoryIndex < system.History().Size()) ++(console->_HistoryIndex);
            }

            // Get history.
            std::string_view prev_cmd = system.History()[console->_HistoryIndex];

            // Insert commands.
            data->InsertChars(data->CursorPos, prev_cmd.data());
        }
        break;

        case ImGuiInputTextFlags_CallbackCharFilter:
        case ImGuiInputTextFlags_CallbackAlways:
        default:
            break;
    }
    return 0;
}