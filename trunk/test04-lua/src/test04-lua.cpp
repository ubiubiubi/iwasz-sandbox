//============================================================================
// Name        : test04-lua.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <lua5.2/lua.hpp>

/**
 * Simple interpretter as found in the book.
 */
void simpleInterpretter (lua_State *L)
{
        char buff[256];
        int error;

        while (fgets(buff, sizeof(buff), stdin) != NULL) {
                error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);

                if (error) {
                        fprintf(stderr, "%s", lua_tostring(L, -1));
                        // pop error message from the stack
                        lua_pop(L, 1);
                }
        }

}

/**
 * Lua error handler. How to connect it to the state?
 */
void error (lua_State *L, const char *fmt, ...)
{
        va_list argp;
        va_start(argp, fmt);
        vfprintf(stderr, fmt, argp);
        va_end(argp);
        lua_close(L);
        exit (EXIT_FAILURE);
}

/**
 * Prints a stack. Taken tfrom the book.
 */
static void stackDump (lua_State *L)
{
        int i;
        int top = lua_gettop(L);
        for (i = 1; i <= top; i++) { /* repeat for each level */
                int t = lua_type(L, i);
                switch (t) {
                case LUA_TSTRING: { /* strings */
                        printf("’%s’", lua_tostring(L, i));
                        break;
                }
                case LUA_TBOOLEAN: { /* booleans */
                        printf(lua_toboolean(L, i) ? "true" : "false");
                        break;
                }
                case LUA_TNUMBER: { /* numbers */
                        printf("%g", lua_tonumber(L, i));
                        break;
                }
                default: { /* other values */
                        printf("%s", lua_typename(L, t));
                        break;
                }
                }
                printf(" "); /* put a separator */
        }
        printf("\n"); /* end the listing */
}

/**
 * Loads and runs a file.
 */
void load (lua_State *L, const char *fname, int *w, int *h)
{
        if (luaL_loadfile (L, fname) || lua_pcall (L, 0, 0, 0)) {
                error (L, "cannot run config. file: %s", lua_tostring (L, -1));
        }

        lua_getglobal (L, "width");
        lua_getglobal (L, "height");

        if (!lua_isnumber (L, -2)) {
                error (L, "’width’ should be a number\n");
        }

        if (!lua_isnumber (L, -1)) {
                error (L, "’height’ should be a number\n");
        }

        *w = lua_tointeger (L, -2);
        *h = lua_tointeger (L, -1);
}

#define MAX_COLOR 255

/* assume that table is on the stack top */
int getfield (lua_State *L, const char *key) {
        int result;

        lua_pushstring (L, key);
        lua_gettable (L, -2); /* get background[key] */

        if (!lua_isnumber (L, -1)) {
                error (L, "invalid component in background color");
        }

        result = lua_tonumber (L, -1) * MAX_COLOR;
        lua_pop (L, 1); /* remove number */
        return result;
}

/*--------------------------------------------------------------------------*/

int main ()
{
        // opens Lua
        lua_State *L = luaL_newstate();
        // opens the standard libraries
        luaL_openlibs(L);

#if 0
        simpleInterpretter (L);
#endif

        int w, h;
        load (L, "file.lua", &w, &h);

        std::cerr << w << ", " << h << std::endl;

        lua_getglobal (L, "background");

        if (!lua_istable (L, -1)) {
                error (L, "’background’ is not a table");
        }

        int red = getfield (L, "r");
        int green = getfield (L, "g");
        int blue = getfield (L, "b");

        std::cerr << red << ", " << green << ", " << blue << std::endl;

        // Close the lua state.
        lua_close(L);
        return 0;
}
