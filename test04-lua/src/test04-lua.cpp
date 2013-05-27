//============================================================================
// Name        : test04-lua.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <lua5.2/lua.hpp>

int main ()
{

        char buff[256];
        int error;

        // opens Lua
        lua_State *L = luaL_newstate();

        // opens the standard libraries
        luaL_openlibs(L);

        while (fgets(buff, sizeof(buff), stdin) != NULL) {
                error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);

                if (error) {
                        fprintf(stderr, "%s", lua_tostring(L, -1));
                        // pop error message from the stack
                        lua_pop(L, 1);

                }
        }

        lua_close(L);
        return 0;
}
