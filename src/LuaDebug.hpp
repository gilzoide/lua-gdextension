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
#ifndef __LUA_DEBUG_HPP__
#define __LUA_DEBUG_HPP__

#include <godot_cpp/classes/ref_counted.hpp>
#include <sol/sol.hpp>

#include "LuaThread.hpp"

using namespace godot;

namespace luagdextension {

class LuaDebug : public RefCounted {
	GDCLASS(LuaDebug, RefCounted);

public:
	LuaDebug();
	LuaDebug(const lua_Debug& debug);
	LuaDebug(lua_Debug&& debug);

	LuaThread::HookEvent get_event() const;
	String get_name() const;
	String get_name_what() const;
	String get_what() const;
	String get_source() const;
	String get_short_src() const;
	int get_line_defined() const;
	int get_last_line_defined() const;
	int get_current_line() const;
#if LUA_VERSION_NUM >= 502
	int get_nparams() const;
	bool is_tail_call() const;
	bool is_vararg() const;
#endif

	static void fill_info(lua_State *L, lua_Debug *ar);
	static void fill_info(const sol::protected_function& f, lua_Debug *ar);

protected:
	static void _bind_methods();
	String _to_string() const;

	lua_Debug debug;
};

}

#endif  // __LUA_DEBUG_HPP__
