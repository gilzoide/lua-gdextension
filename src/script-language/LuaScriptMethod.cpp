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
#include "LuaScriptMethod.hpp"

#include "../LuaDebug.hpp"
#include "../LuaFunction.hpp"
#include "../utils/stack_top_checker.hpp"

namespace luagdextension {

LuaScriptMethod::LuaScriptMethod(const StringName& name, sol::protected_function method)
	: name(name)
	, method(method)
{
}

bool LuaScriptMethod::is_valid() const {
	return method.valid();
}

int LuaScriptMethod::get_line_defined() const {
#ifdef DEBUG_ENABLED
	return LuaObject::wrap_object<LuaFunction>(method)->get_debug_info()->get_line_defined();
#else
	return -1;
#endif
}

Variant LuaScriptMethod::get_argument_count() const {
#if defined(DEBUG_ENABLED) && LUA_VERSION_NUM >= 502
	return LuaObject::wrap_object<LuaFunction>(method)->get_debug_info()->get_nparams();
#else
	return {};
#endif
}

MethodInfo LuaScriptMethod::to_method_info() const {
	MethodInfo mi;
	mi.name = name;

#if defined(DEBUG_ENABLED) && LUA_VERSION_NUM >= 502
	sol::state_view state = LuaObject::wrap_object<LuaFunction>(method)->get_function().lua_state();
	StackTopChecker topcheck(state);

	auto debug_info = LuaObject::wrap_object<LuaFunction>(method)->get_debug_info();
	auto methodpop = sol::stack::push_pop(state, method);
	for (int i = 0; i < debug_info->get_nparams(); i++) {
		String arg_name = lua_getlocal(state, nullptr, i + 1);
		if (i == 0 && arg_name == "self") {
			continue;
		}
		mi.arguments.push_back(PropertyInfo(Variant::Type::NIL, arg_name));
	}
	if (debug_info->is_vararg()) {
		mi.flags |= GDEXTENSION_METHOD_FLAG_VARARG;
	}
#endif

	return mi;
}

Dictionary LuaScriptMethod::to_dictionary() const {
	return to_method_info();
}

void LuaScriptMethod::register_lua(lua_State *L) {
	
}

}
