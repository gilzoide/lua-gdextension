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
#ifndef __LUA_THREAD_HPP__
#define __LUA_THREAD_HPP__

#include "LuaObject.hpp"

using namespace godot;

namespace luagdextension {

class LuaDebug;

class LuaThread : public LuaObjectSubclass<sol::thread> {
	GDCLASS(LuaThread, LuaObject);

public:
	enum Status {
		STATUS_OK = LUA_OK,
		STATUS_YIELD = LUA_YIELD,
		STATUS_ERRRUN = LUA_ERRRUN,
		STATUS_ERRSYNTAX = LUA_ERRSYNTAX,
		STATUS_ERRMEM = LUA_ERRMEM,
		STATUS_ERRERR = LUA_ERRERR,
		STATUS_DEAD = (int) sol::thread_status::dead,
	};
	
	enum HookMask {
		HOOK_MASK_CALL = LUA_MASKCALL,
		HOOK_MASK_RETURN = LUA_MASKRET,
		HOOK_MASK_LINE = LUA_MASKLINE,
		HOOK_MASK_COUNT = LUA_MASKCOUNT,
	};

	enum HookEvent {
		HOOK_EVENT_CALL = LUA_HOOKCALL,
		HOOK_EVENT_RETURN = LUA_HOOKRET,
		HOOK_EVENT_LINE = LUA_HOOKLINE,
		HOOK_EVENT_COUNT = LUA_HOOKCOUNT,
#if LUA_VERSION_NUM >= 502
		HOOK_EVENT_TAIL_CALL = LUA_HOOKTAILCALL,
		HOOK_EVENT_TAIL_RETURN = LUA_HOOKTAILCALL,
#else
		HOOK_EVENT_TAIL_CALL = LUA_HOOKTAILRET,
		HOOK_EVENT_TAIL_RETURN = LUA_HOOKTAILRET,
#endif
	};

	enum HookResult {
		HOOK_OK = 0,
		HOOK_YIELD = -1,
	};

	LuaThread();
	LuaThread(sol::thread&& thread);
	LuaThread(const sol::thread& thread);

	Status get_status() const;
	bool is_main_thread() const;

	void set_hook(Variant hook, BitField<HookMask> mask, int count = 0);
	Variant get_hook() const;
	BitField<HookMask> get_hook_mask() const;
	int get_hook_count() const;

	Ref<LuaDebug> get_stack_level_info(int stack_level) const;
	TypedArray<LuaDebug> get_stack_info() const;
	String get_traceback(String message = "", int level = 0) const;
	
protected:
	static void _bind_methods();
};

}
VARIANT_ENUM_CAST(luagdextension::LuaThread::HookEvent);
VARIANT_ENUM_CAST(luagdextension::LuaThread::HookResult);
VARIANT_ENUM_CAST(luagdextension::LuaThread::Status);
VARIANT_BITFIELD_CAST(luagdextension::LuaThread::HookMask);

#endif  // __LUA_THREAD_HPP__
