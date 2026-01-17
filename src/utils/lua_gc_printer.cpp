/**
 * Copyright (C) 2026 Gil Barbosa Reis.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef DEBUG_ENABLED

#include "lua_gc_printer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

namespace luagdextension {

long long get_lua_gcbytes(lua_State *L) {
	return lua_gc(L, LUA_GCCOUNT, 0) * 1024
		+ lua_gc(L, LUA_GCCOUNTB, 0);
}

LuaGcPrinter::LuaGcPrinter(lua_State *L)
	: L(L)
	, previous(get_lua_gcbytes(L))
{
}

LuaGcPrinter::~LuaGcPrinter() {
	int64_t current = get_lua_gcbytes(L);
	int64_t delta = current - previous;
	UtilityFunctions::print("LuaGcPrinter: ", delta);
}

}

#endif
