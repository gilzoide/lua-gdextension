/**
 * Copyright (C) 2025 Gil Barbosa Reis.
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
#ifndef __LUA_SCRIPT_METHOD_HPP__
#define __LUA_SCRIPT_METHOD_HPP__

#include <sol/sol.hpp>
#include <godot_cpp/core/object.hpp>

typedef struct lua_State lua_State;

using namespace godot;

namespace luagdextension {

struct LuaScriptMethod {
	StringName name;
	sol::protected_function method;
	
	LuaScriptMethod() = default;
	LuaScriptMethod(const StringName& name, sol::protected_function method);

	bool is_valid() const;
	int get_line_defined() const;
	Variant get_argument_count() const;

	MethodInfo to_method_info() const;
	Dictionary to_dictionary() const;

	static void register_lua(lua_State *L);
};

}

#endif  // __LUA_SCRIPT_METHOD_HPP__
