#pragma once

#include <string_view>

#include "platform/defines.hpp"
#include "platform/lua.hpp"
#include "core/exception.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(FailedToExecuteScript);
    }

    struct ACDA_API ScriptInterpreter
    {
    public:
        using SelfType = ScriptInterpreter;
    public:
        ScriptInterpreter();
        ~ScriptInterpreter();

        ScriptInterpreter(const SelfType&) = delete;
        auto operator=(const SelfType&) -> SelfType & = delete;

        void ExecuteScript(std::string_view text);
    private:
        lua_State* _LuaState{ nullptr };
    };
}