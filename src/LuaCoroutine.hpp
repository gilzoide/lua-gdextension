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
#ifndef __LUA_COROUTINE_HPP__
#define __LUA_COROUTINE_HPP__

#include "LuaObject.hpp"

#include <gdextension_interface.h>

using namespace godot;

namespace luagdextension {

class LuaFunction;

class LuaCoroutine : public LuaObject {
	GDCLASS(LuaCoroutine, LuaObject);

public:
	enum LuaCoroutineStatus {
		STATUS_OK = LUA_OK,
		STATUS_YIELD = LUA_YIELD,
		STATUS_ERRRUN = LUA_ERRRUN,
		STATUS_ERRSYNTAX = LUA_ERRSYNTAX,
		STATUS_ERRMEM = LUA_ERRMEM,
		STATUS_ERRERR = LUA_ERRERR,
		STATUS_DEAD = (int) sol::thread_status::dead,
	};

	LuaCoroutine() = default;
	LuaCoroutine(sol::thread&& thread);
	LuaCoroutine(const sol::thread& thread);
	~LuaCoroutine();

	static LuaCoroutine *create(const sol::function& function);
	static LuaCoroutine *create(LuaFunction *function);

	LuaCoroutineStatus get_status() const;
	Variant resumev(const Array& args);
	Variant resume(const Variant **argv, GDExtensionInt argc, GDExtensionCallError& error);

	sol::object get_lua_object() const override;

	operator String() const;

protected:
	static void _bind_methods();
	
	String _to_string() const;

	sol::thread thread;
};

}
VARIANT_ENUM_CAST(luagdextension::LuaCoroutine::LuaCoroutineStatus);

#endif
