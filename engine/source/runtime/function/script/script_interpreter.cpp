#include "script_interpreter.hpp"

Arcadia::ScriptInterpreter::ScriptInterpreter()
{
    _LuaState = luaL_newstate();
    luaL_openlibs(_LuaState);
}

Arcadia::ScriptInterpreter::~ScriptInterpreter()
{
    lua_close(_LuaState);
    _LuaState = nullptr;
}

void Arcadia::ScriptInterpreter::ExecuteScript(std::string_view text)
{
    int error = luaL_loadbuffer(_LuaState, text.data(), text.size(), NULL) || lua_pcall(_LuaState, 0, 0, 0);
    if(error)
    {
        std::string error_msg = lua_tostring(_LuaState, -1);
        lua_pop(_LuaState, 1);
        throw Exceptions::FailedToExecuteScript(error_msg);
    }
}