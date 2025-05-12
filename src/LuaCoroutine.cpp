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
#include "LuaCoroutine.hpp"

#include "LuaError.hpp"
#include "LuaFunction.hpp"
#include "utils/VariantArguments.hpp"
#include "utils/convert_godot_lua.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

LuaCoroutine::LuaCoroutine() : LuaObjectSubclass() {}
LuaCoroutine::LuaCoroutine(sol::thread&& thread) : LuaObjectSubclass(thread) {}
LuaCoroutine::LuaCoroutine(const sol::thread& thread) : LuaObjectSubclass(thread) {}

LuaCoroutine *LuaCoroutine::create(const sol::function& function) {
	sol::thread thread = sol::thread::create(function.lua_state());
	function.push(thread.thread_state());
	return memnew(LuaCoroutine(thread));
}

LuaCoroutine *LuaCoroutine::create(LuaFunction *function) {
	ERR_FAIL_COND_V_MSG(!function, nullptr, "Function cannot be null");
	return create(function->get_function());
}

LuaCoroutine::Status LuaCoroutine::get_status() const {
	return static_cast<Status>(lua_object.status());
}

Variant LuaCoroutine::resume(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError& error) {
	ERR_FAIL_COND_V_MSG(lua_object.status() != sol::thread_status::yielded, Variant(), "Cannot resume a coroutine that is not suspended.");
	error.error = GDEXTENSION_CALL_OK;
	return _resume(VariantArguments(args, arg_count), true);
}

Variant LuaCoroutine::resumev(const Array& args) {
	ERR_FAIL_COND_V_MSG(lua_object.status() != sol::thread_status::yielded, Variant(), "Cannot resume a coroutine that is not suspended.");
	return _resume(VariantArguments(args), true);
}

Variant LuaCoroutine::_resume(const VariantArguments& args, bool return_lua_error) {
	lua_State *L = lua_object.thread_state();
	sol::stack::push(L, args);

	int nresults;
	int status = lua_resume(L, nullptr, args.argc(), &nresults);
	sol::protected_function_result function_result(L, -nresults, nresults, nresults, static_cast<sol::call_status>(status));
	Variant ret = to_variant(function_result, true);
	if (status == LUA_OK) {
		emit_signal("completed", ret);
	}
	else if (status != LUA_YIELD) {
		emit_signal("failed", ret);
		if (!return_lua_error) {
			return Variant();
		}
	}
	return ret;
}

Variant LuaCoroutine::invoke_lua(const sol::protected_function& f, const VariantArguments& args, bool return_lua_error) {
	Ref<LuaCoroutine> coroutine = LuaCoroutine::create(f);
	Variant result = coroutine->_resume(args, return_lua_error);
	if (coroutine->get_status() == LuaCoroutine::STATUS_YIELD) {
		return coroutine;
	}
	else {
		return result;
	}
}

void LuaCoroutine::_bind_methods() {
	BIND_ENUM_CONSTANT(STATUS_OK);
	BIND_ENUM_CONSTANT(STATUS_YIELD);
	BIND_ENUM_CONSTANT(STATUS_ERRRUN);
	BIND_ENUM_CONSTANT(STATUS_ERRSYNTAX);
	BIND_ENUM_CONSTANT(STATUS_ERRMEM);
	BIND_ENUM_CONSTANT(STATUS_ERRERR);
	BIND_ENUM_CONSTANT(STATUS_DEAD);

	ClassDB::bind_method(D_METHOD("get_status"), &LuaCoroutine::get_status);
	ClassDB::bind_method(D_METHOD("resumev", "arguments"), &LuaCoroutine::resumev);
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "resume", &LuaCoroutine::resume);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("create", "function"), sol::resolve<LuaCoroutine *(LuaFunction *)>(&LuaCoroutine::create));

	ADD_PROPERTY(PropertyInfo(Variant::INT, "status", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CLASS_IS_ENUM, "Status"), "", "get_status");

	ADD_SIGNAL(MethodInfo("completed", PropertyInfo(Variant::NIL, "result", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NIL_IS_VARIANT)));
	ADD_SIGNAL(MethodInfo("failed", PropertyInfo(Variant::OBJECT, "error", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, LuaError::get_class_static())));
}

}
