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
#include "LuaThread.hpp"
#include "LuaDebug.hpp"
#include "LuaFunction.hpp"
#include "utils/convert_godot_lua.hpp"
#include "utils/stack_top_checker.hpp"

namespace luagdextension {

static const char *HOOKKEY = "_GD_HOOKKEY";

static void hookf(lua_State *L, lua_Debug *ar) {
	lua_getfield(L, LUA_REGISTRYINDEX, HOOKKEY);
	lua_pushthread(L);
	switch (lua_rawget(L, -2)) {
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
			LuaDebug::fill_info(L, ar);
			lua_push(L, memnew(LuaDebug(*ar)));
			lua_call(L, 1, 1);
			if (lua_tointeger(L, -1) == LuaThread::HOOK_YIELD) {
				lua_yield(L, 0);
			}
			lua_pop(L, 1);
			break;
		
		default:
			break;
	}
}

LuaThread::LuaThread() : LuaObjectSubclass() {}
LuaThread::LuaThread(sol::thread&& thread) : LuaObjectSubclass(thread) {}
LuaThread::LuaThread(const sol::thread& thread) : LuaObjectSubclass(thread) {}

LuaThread::Status LuaThread::get_status() const {
	return static_cast<Status>(lua_object.status());
}

bool LuaThread::is_main_thread() const {
	return lua_object.is_main_thread();
}

void LuaThread::set_hook(Variant hook, BitField<HookMask> mask, int count) {
	lua_State *L = lua_object.thread_state();
	switch (hook.get_type()) {
		case Variant::Type::NIL:
			break;

		case Variant::Type::OBJECT:
			if (hook && !hook.operator Object*()->is_class(LuaFunction::get_class_static())) {
				ERR_FAIL_MSG("Hook should be either null, Callable or LuaFunction");
			}
			break;

		case Variant::Type::CALLABLE:
			ERR_FAIL_COND_MSG(!Callable(hook).is_valid(), "Hook is not a valid Callable");
			break;

		default:
			ERR_FAIL_MSG("Hook should be either null, Callable or LuaFunction");
	}

	StackTopChecker topcheck(L);
	if (!luaL_getsubtable(L, LUA_REGISTRYINDEX, HOOKKEY)) {
		/* table just created; initialize it */
		lua_pushliteral(L, "k");
		lua_setfield(L, -2, "__mode");  // hooktable.__mode = "k"
		lua_pushvalue(L, -1);
		lua_setmetatable(L, -2);  // metatable(hooktable) = hooktable
	}
	lua_pushthread(L);  // key (thread)
	lua_push(L, hook);  // value (hook)
	lua_rawset(L, -3);  // hooktable[L] = hook
	lua_pop(L, 1);
	if (hook && mask) {
		lua_sethook(L, hookf, mask, count);
	}
	else {
		lua_sethook(L, nullptr, 0, 0);
	}
}

Variant LuaThread::get_hook() const {
	sol::state_view L(lua_object.thread_state());
	if (auto hook = L.registry().traverse_get<sol::optional<sol::object>>(HOOKKEY, lua_object)) {
		return to_variant(*hook);
	}
	else {
		return {};
	}
}

BitField<LuaThread::HookMask> LuaThread::get_hook_mask() const {
	return lua_gethookmask(lua_object.thread_state());
}

int LuaThread::get_hook_count() const {
	return lua_gethookcount(lua_object.thread_state());
}

Ref<LuaDebug> LuaThread::get_stack_level_info(int stack_level) const {
	lua_State *L = lua_object.thread_state();
	lua_Debug debug = {};
	if (lua_getstack(L, stack_level, &debug)) {
		LuaDebug::fill_info(L, &debug);
		return memnew(LuaDebug(std::move(debug)));
	}
	else {
		return nullptr;
	}
}

TypedArray<LuaDebug> LuaThread::get_stack_info() const {
	TypedArray<LuaDebug> stack;
	for (int i = 0; ; i++) {
		Ref<LuaDebug> stack_level = get_stack_level_info(i);
		if (stack_level.is_valid()) {
			stack.append(stack_level);
		}
		else {
			break;
		}
	}
	return stack;
}

String LuaThread::get_traceback(String message, int level) const {
	lua_State *L = lua_object.lua_state();
	lua_State *L1 = lua_object.thread_state();
	StackTopChecker topcheck(L);
	luaL_traceback(L, L1, message.ascii().get_data(), level);
	String result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

void LuaThread::_bind_methods() {
	BIND_ENUM_CONSTANT(STATUS_OK);
	BIND_ENUM_CONSTANT(STATUS_YIELD);
	BIND_ENUM_CONSTANT(STATUS_ERRRUN);
	BIND_ENUM_CONSTANT(STATUS_ERRSYNTAX);
	BIND_ENUM_CONSTANT(STATUS_ERRMEM);
	BIND_ENUM_CONSTANT(STATUS_ERRERR);
	BIND_ENUM_CONSTANT(STATUS_DEAD);

	BIND_BITFIELD_FLAG(HOOK_MASK_CALL);
	BIND_BITFIELD_FLAG(HOOK_MASK_RETURN);
	BIND_BITFIELD_FLAG(HOOK_MASK_LINE);
	BIND_BITFIELD_FLAG(HOOK_MASK_COUNT);

	BIND_ENUM_CONSTANT(HOOK_EVENT_CALL);
	BIND_ENUM_CONSTANT(HOOK_EVENT_RETURN);
	BIND_ENUM_CONSTANT(HOOK_EVENT_LINE);
	BIND_ENUM_CONSTANT(HOOK_EVENT_COUNT);
	BIND_ENUM_CONSTANT(HOOK_EVENT_TAIL_CALL);
	BIND_ENUM_CONSTANT(HOOK_EVENT_TAIL_RETURN);

	BIND_ENUM_CONSTANT(HOOK_OK);
	BIND_ENUM_CONSTANT(HOOK_YIELD);

	ClassDB::bind_method(D_METHOD("get_status"), &LuaThread::get_status);
	ClassDB::bind_method(D_METHOD("is_main_thread"), &LuaThread::is_main_thread);
	
	ClassDB::bind_method(D_METHOD("set_hook", "hook", "mask", "count"), &LuaThread::set_hook, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_hook"), &LuaThread::get_hook);
	ClassDB::bind_method(D_METHOD("get_hook_mask"), &LuaThread::get_hook_mask);
	ClassDB::bind_method(D_METHOD("get_hook_count"), &LuaThread::get_hook_count);
	
	ClassDB::bind_method(D_METHOD("get_stack_level_info", "level"), &LuaThread::get_stack_level_info);
	ClassDB::bind_method(D_METHOD("get_stack_info"), &LuaThread::get_stack_info);
	ClassDB::bind_method(D_METHOD("get_traceback", "message", "level"), &LuaThread::get_traceback, DEFVAL(""), DEFVAL(0));

	ADD_PROPERTY(PropertyInfo(Variant::INT, "status", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CLASS_IS_ENUM, "Status"), "", "get_status");
}

}
