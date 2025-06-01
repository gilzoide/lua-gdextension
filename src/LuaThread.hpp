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
#ifndef __LUA_THREAD_HPP__
#define __LUA_THREAD_HPP__

#include "LuaObject.hpp"

using namespace godot;

namespace luagdextension {

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
	
protected:
	static void _bind_methods();
};

}
VARIANT_ENUM_CAST(luagdextension::LuaThread::Status);
VARIANT_ENUM_CAST(luagdextension::LuaThread::HookResult);
VARIANT_BITFIELD_CAST(luagdextension::LuaThread::HookMask);

#endif  // __LUA_THREAD_HPP__
