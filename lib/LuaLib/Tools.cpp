#include "Tools.h"


void SetCFunction(lua_State *lua, int(*CFunc)(lua_State *lua),const char* funcName)
{
	lua_pushcfunction(lua, CFunc);
	lua_setglobal(lua, funcName);
}

void SetCFunction(lua_State *lua, int(*CFunc)(lua_State *lua), const char* packageName, const char* funcName)
{
	luaL_Reg apis[] = { { funcName, CFunc }, { NULL, NULL } };
	luaL_openlib(lua, packageName, apis, 0);//package name
}