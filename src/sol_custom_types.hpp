/**
 * Copyright (C) 2023 Gil Barbosa Reis.
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
#ifndef __SOL_CUSTOM_TYPES_HPP__
#define __SOL_CUSTOM_TYPES_HPP__

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <sol/sol.hpp>

using namespace godot;

/// Custom String <-> Lua string conversions
template<typename Handler>
bool sol_lua_check(sol::types<String>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking) {
	bool success = sol::stack::check<const char *>(L, index, handler);
	tracking.use(1);
	return success;
}
String sol_lua_get(sol::types<String>, lua_State* L, int index, sol::stack::record& tracking);
int sol_lua_push(lua_State* L, const String& str);

/// Custom StringName <-> Lua string conversions
template<typename Handler>
bool sol_lua_check(sol::types<StringName>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking) {
	bool success = sol::stack::check<const char *>(L, index, handler);
	tracking.use(1);
	return success;
}
StringName sol_lua_get(sol::types<StringName>, lua_State* L, int index, sol::stack::record& tracking);
int sol_lua_push(lua_State* L, const StringName& str);

#endif
