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

#include "script-language/LuaScriptInstance.hpp"
#include "utils/convert_godot_lua.hpp"

#include <godot_cpp/core/error_macros.hpp>

namespace luagdextension {

LuaFunction::LuaFunction() : LuaObjectSubclass() {}
LuaFunction::LuaFunction(sol::protected_function&& function) : LuaObjectSubclass(function) {}
LuaFunction::LuaFunction(const sol::protected_function& function) : LuaObjectSubclass(function) {}

void LuaFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invokev", "arg_array"), &LuaFunction::invokev);
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "invoke", &LuaFunction::invoke);
}

Variant LuaFunction::invokev(const Array& args) {
	int arg_count = args.size();
	if (arg_count == 0) {
		return to_variant(lua_object.call());
	}

	lua_State *L = lua_object.lua_state();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(lua_object.call(lua_args));
}

Variant LuaFunction::invoke(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	error.error = GDEXTENSION_CALL_OK;
	if (arg_count == 0) {
		return to_variant(lua_object.call());
	}

	lua_State *L = lua_object.lua_state();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, *args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(lua_object.call(lua_args));
}

Variant LuaFunction::invoke_method(LuaScriptInstance *self, const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	if (!self) {
		error.error = GDEXTENSION_CALL_ERROR_INSTANCE_IS_NULL;
		return {};
	}
	error.error = GDEXTENSION_CALL_OK;

	lua_State *L = lua_object.lua_state();
	std::ignore = to_lua(L, self);
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, *args[i]);
	}
	sol::variadic_args lua_args(L, -(1 + arg_count));
	return to_variant(lua_object.call(lua_args));
}

Callable LuaFunction::to_callable() const {
	return Callable((Object *) this, "invoke");
}

const sol::protected_function& LuaFunction::get_function() const {
	return lua_object;
}

}
