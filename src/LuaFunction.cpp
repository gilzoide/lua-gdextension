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
#include "LuaFunction.hpp"

#include "LuaState.hpp"
#include "utils/convert_godot_lua.hpp"

#include <godot_cpp/core/error_macros.hpp>

namespace luagdextension {

LuaFunction::LuaFunction() : function() {
	ERR_PRINT("LuaUserdata should never be instantiated manually!");
}
LuaFunction::LuaFunction(sol::protected_function&& function) : function(function) {}
LuaFunction::LuaFunction(const sol::protected_function& function) : function(function) {}

LuaFunction::~LuaFunction() {
	if (function.valid() && LuaState::is_valid(function.lua_state())) {
		function.~basic_protected_function();
	}
}

sol::object LuaFunction::get_lua_object() const {
	return function;
}

void LuaFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invokev", "arg_array"), &LuaFunction::invokev);
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "invoke", &LuaFunction::invoke);
}

Variant LuaFunction::invokev(const Array& args) {
	ERR_FAIL_COND_V_EDMSG(!function.valid(), Variant(), "LuaFunction does not have a valid function");
	int arg_count = args.size();
	if (arg_count == 0) {
		return to_variant(function.call());
	}

	lua_State *L = function.lua_state();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(function.call(lua_args));
}

Variant LuaFunction::invoke(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	ERR_FAIL_COND_V_EDMSG(!function.valid(), Variant(), "LuaFunction does not have a valid function");
	if (arg_count == 0) {
		return to_variant(function.call());
	}

	lua_State *L = function.lua_state();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, *args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(function.call(lua_args));
}

const sol::protected_function& LuaFunction::get_function() const {
	return function;
}

LuaFunction::operator String() const {
	return _to_string();
}

String LuaFunction::_to_string() const {
	return String("[LuaFunction:0x%x]") % (int64_t) function.pointer();
}

}
