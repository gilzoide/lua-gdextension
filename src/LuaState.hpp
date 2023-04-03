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
#ifndef __LUA_STATE_HPP__
#define __LUA_STATE_HPP__

#include "LuaTable.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

class LuaState : public LuaTable {
	GDCLASS(LuaState, LuaTable);

public:
	enum Library {
		// ----- Lua/LuaJIT ----
		// print, assert, and other base functions
		BASE = 1 << 0,
		// require and other package functions
		PACKAGE = 1 << 1,
		// coroutine functions and utilities
		COROUTINE = 1 << 2,
		// string library
		STRING = 1 << 3,
		// functionality from the OS
		OS = 1 << 4,
		// all things math
		MATH = 1 << 5,
		// the table manipulator and observer functions
		TABLE = 1 << 6,
		// the debug library
		DEBUG = 1 << 7,
		// the bit library: different based on which you're using
		BIT32 = 1 << 8,
		// input/output library
		IO = 1 << 9,
		// LuaJIT only
		FFI = 1 << 10,
		// LuaJIT only
		JIT = 1 << 11,
		// library for handling utf8: new to Lua
		UTF8 = 1 << 12,

		// all of the above
		LUA = BASE | PACKAGE | COROUTINE | STRING | OS | MATH | TABLE | DEBUG | BIT32 | IO | FFI | JIT | UTF8,

		// ----- Godot ----
	};

	LuaState();

	void open_libraries(BitField<Library> libraries);

	Variant do_string(const String& chunk, const String& chunkname = "");
	Variant do_file(const String& filename, int buffer_size = 1024);

	LuaTable *get_globals() const;
	LuaTable *get_registry() const;

protected:
	static void _bind_methods();

	String _to_string() const;

private:
	sol::state lua_state;
};

}
VARIANT_BITFIELD_CAST(luagdextension::LuaState::Library);

#endif
