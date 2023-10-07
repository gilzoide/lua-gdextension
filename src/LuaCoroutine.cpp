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
#include "LuaCoroutine.hpp"

#include "LuaFunction.hpp"
#include "utils/convert_godot_lua.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

LuaCoroutine::LuaCoroutine(sol::coroutine&& coroutine) : coroutine(coroutine) {}
LuaCoroutine::LuaCoroutine(const sol::coroutine& coroutine) : coroutine(coroutine) {}

LuaCoroutine *LuaCoroutine::create(LuaFunction *function) {
	ERR_FAIL_COND_V_MSG(!function, nullptr, "Function cannot be null");
	return memnew(LuaCoroutine(function->get_function()));
}

LuaCoroutine::LuaCoroutineStatus LuaCoroutine::get_status() const {
	return (LuaCoroutineStatus) coroutine.status();
}

Variant LuaCoroutine::resume(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError& error) {
	lua_State *L = coroutine.lua_state();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, *args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(coroutine.call(lua_args));
}

Variant LuaCoroutine::resumev(const Array& args) {
	lua_State *L = coroutine.lua_state();
	int arg_count = args.size();
	for (int i = 0; i < arg_count; i++) {
		std::ignore = to_lua(L, args[i]);
	}
	sol::variadic_args lua_args(L, -arg_count);
	return to_variant(coroutine.call(lua_args));
}

LuaCoroutine::operator String() const {
	return _to_string();
}

void LuaCoroutine::_bind_methods() {
	BIND_ENUM_CONSTANT(STATUS_OK);
	BIND_ENUM_CONSTANT(STATUS_YIELD);
	BIND_ENUM_CONSTANT(STATUS_ERRRUN);
	BIND_ENUM_CONSTANT(STATUS_ERRMEM);
	BIND_ENUM_CONSTANT(STATUS_ERRGCMM);
	BIND_ENUM_CONSTANT(STATUS_ERRERR);
	BIND_ENUM_CONSTANT(STATUS_SYNTAX);
	BIND_ENUM_CONSTANT(STATUS_FILE);

	ClassDB::bind_method(D_METHOD("get_status"), &LuaCoroutine::get_status);
	ClassDB::bind_method(D_METHOD("resumev", "arguments"), &LuaCoroutine::resumev);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("create", "function"), sol::resolve<LuaCoroutine *(LuaFunction *)>(&LuaCoroutine::create));
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "resume", &LuaCoroutine::resume);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "status"), "", "get_status");
}

String LuaCoroutine::_to_string() const {
	return UtilityFunctions::str("[LuaCoroutine:0x", String::num_uint64((uint64_t) coroutine.pointer(), 16), "]");
}

}
