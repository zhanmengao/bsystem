#pragma once
#include <iostream>

//引入c程序
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void SetCFunction(lua_State *lua, int(*CFunc)(lua_State *lua),const char* funcName);

void SetCFunction(lua_State *lua, int(*CFunc)(lua_State *lua), const char* packageName, const char* funcName);