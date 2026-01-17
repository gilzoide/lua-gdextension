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
#include "LuaCoroutine.hpp"

#include "LuaError.hpp"
#include "LuaFunction.hpp"
#include "utils/LuaCoroutinePool.hpp"
#include "utils/VariantArguments.hpp"
#include "utils/convert_godot_lua.hpp"
#include "utils/string_names.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

LuaCoroutine::LuaCoroutine() : LuaThread() {}
LuaCoroutine::LuaCoroutine(sol::thread&& thread) : LuaThread(thread) {}
LuaCoroutine::LuaCoroutine(const sol::thread& thread) : LuaThread(thread) {}

LuaCoroutine *LuaCoroutine::create(const sol::function& function) {
	sol::thread thread = sol::thread::create(function.lua_state());
	function.push(thread.thread_state());
	return memnew(LuaCoroutine(thread));
}

LuaCoroutine *LuaCoroutine::create(LuaFunction *function) {
	ERR_FAIL_COND_V_MSG(function == nullptr, nullptr, "Function cannot be null");
	return create(function->get_function());
}

Variant LuaCoroutine::resume(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError& error) {
	error.error = GDEXTENSION_CALL_OK;
	return _resume(VariantArguments(args, arg_count), true);
}

Variant LuaCoroutine::resumev(const Array& args) {
	return _resume(VariantArguments(args), true);
}

Variant LuaCoroutine::_resume(const VariantArguments& args, bool return_lua_error) {
	ERR_FAIL_COND_V_MSG(lua_object.status() != sol::thread_status::yielded, Variant(), "Cannot resume a coroutine that is not suspended.");
	sol::protected_function_result function_result = _resume(lua_object.thread_state(), args);
	Variant ret = to_variant(function_result, true);
	if (function_result.status() == sol::call_status::ok) {
		emit_signal(string_names->completed, ret);
	}
	else if (function_result.status() != sol::call_status::yielded) {
		emit_signal(string_names->failed, ret);
		if (!return_lua_error) {
			return Variant();
		}
	}
	return ret;
}

sol::protected_function_result LuaCoroutine::_resume(lua_State *L, const VariantArguments& args) {
	sol::stack::push(L, args);

	int nresults;
	int status = resume_lua_coroutine(L, args.argc(), &nresults);
	sol::protected_function_result function_result(L, -nresults, nresults, nresults, static_cast<sol::call_status>(status));
	return function_result;
}

Variant LuaCoroutine::invoke_lua(Ref<LuaFunction> f, const VariantArguments& args, bool return_lua_error) {
	return invoke_lua(f->get_function(), args, return_lua_error);
}

Variant LuaCoroutine::invoke_lua(const sol::protected_function& f, const VariantArguments& args, bool return_lua_error) {
	LuaCoroutinePool pool(f.lua_state());
	sol::thread coroutine = pool.acquire(f);
	sol::protected_function_result result = _resume(coroutine.thread_state(), args);
	if (result.status() == sol::call_status::yielded) {
		return LuaObject::wrap_object<LuaCoroutine>(coroutine);
	}
	else {
		pool.release(coroutine);
		return to_variant(result, return_lua_error);
	}
}

void LuaCoroutine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("resumev", "arguments"), &LuaCoroutine::resumev);
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "resume", &LuaCoroutine::resume);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("create", "function"), sol::resolve<LuaCoroutine *(LuaFunction *)>(&LuaCoroutine::create));

	ADD_SIGNAL(MethodInfo(string_names->completed, PropertyInfo(Variant::NIL, "result", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NIL_IS_VARIANT)));
	ADD_SIGNAL(MethodInfo(string_names->failed, PropertyInfo(Variant::OBJECT, "error", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, LuaError::get_class_static())));
}

}
