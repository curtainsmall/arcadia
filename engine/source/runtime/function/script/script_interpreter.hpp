#pragma once

#include <string_view>

#include "core/noncopyable.hpp"
#include "platform/defines.hpp"
#include "platform/lua.hpp"
#include "core/exception.hpp"

namespace Arcadia
{
    namespace Exceptions
    {
        ACDA_DEFINE_EXCEPTION(FailedToExecuteScript);
    }

    struct ACDA_API ScriptInterpreter: public Noncopyable
    {
    public:
        ScriptInterpreter();
        ~ScriptInterpreter();

        void ExecuteScript(std::string_view text);
    private:
        lua_State* _LuaState{ nullptr };
    };
}