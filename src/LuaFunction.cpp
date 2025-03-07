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

#include "LuaError.hpp"
#include "script-language/LuaScriptInstance.hpp"
#include "utils/VariantArguments.hpp"
#include "utils/convert_godot_lua.hpp"

#include <godot_cpp/core/error_macros.hpp>

namespace luagdextension {

static Variant result_value(const sol::protected_function_result& result, bool return_lua_error) {
	if (!return_lua_error && !result.valid()) {
		ERR_PRINT(LuaError::extract_message(result));
		return Variant();
	}
	else {
		return to_variant(result);
	}
}

LuaFunction::LuaFunction() : LuaObjectSubclass() {}
LuaFunction::LuaFunction(sol::protected_function&& function) : LuaObjectSubclass(function) {}
LuaFunction::LuaFunction(const sol::protected_function& function) : LuaObjectSubclass(function) {}

void LuaFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invokev", "arg_array"), &LuaFunction::invokev);
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "invoke", &LuaFunction::invoke);
}

Variant LuaFunction::invokev(const Array& args) {
	return invokev_lua(lua_object, args, true);
}

Variant LuaFunction::invoke(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	error.error = GDEXTENSION_CALL_OK;
	return invoke_lua(lua_object, args, arg_count, true);
}

Variant LuaFunction::invoke_method(const Variant& self, const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	if (!self) {
		error.error = GDEXTENSION_CALL_ERROR_INSTANCE_IS_NULL;
		return {};
	}
	error.error = GDEXTENSION_CALL_OK;

	return invoke_method_lua(lua_object, self, args, arg_count, false);
}

Variant LuaFunction::invoke_method(LuaScriptInstance *self, const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	return invoke_method(self->owner, args, arg_count, error);
}

Variant LuaFunction::invoke_lua(const sol::protected_function& f, const Variant **args, GDExtensionInt arg_count, bool return_lua_error) {
	lua_State *L = f.lua_state();
	sol::protected_function_result result = f.call(VariantArguments(args, arg_count));
	return result_value(result, return_lua_error);
}

Variant LuaFunction::invoke_method_lua(const sol::protected_function& f, const Variant& self, const Variant **args, GDExtensionInt arg_count, bool return_lua_error) {
	lua_State *L = f.lua_state();
	sol::protected_function_result result = f.call(VariantArguments(self, args, arg_count));
	return result_value(result, return_lua_error);
}

Variant LuaFunction::invokev_lua(const sol::protected_function& f, const Array& args, bool return_lua_error) {
	lua_State *L = f.lua_state();
	sol::protected_function_result result = f.call(VariantArguments(args));
	return result_value(result, return_lua_error);
}

Callable LuaFunction::to_callable() const {
	return Callable((Object *) this, "invoke");
}

const sol::protected_function& LuaFunction::get_function() const {
	return lua_object;
}

}
