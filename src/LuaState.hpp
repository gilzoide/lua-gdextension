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

#include "utils/custom_sol.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace luagdextension {

class LuaTable;

/**
 * Lua memory allocation callback.
 * Uses Godot memory functions.
 */
void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);

class LuaState : public RefCounted {
	GDCLASS(LuaState, RefCounted);

public:
	enum Library {
		// ----- Lua/LuaJIT ----
		// print, assert, and other base functions
		LUA_BASE = 1 << 0,
		// require and other package functions
		LUA_PACKAGE = 1 << 1,
		// coroutine functions and utilities
		LUA_COROUTINE = 1 << 2,
		// string library
		LUA_STRING = 1 << 3,
		// functionality from the OS
		LUA_OS = 1 << 4,
		// all things math
		LUA_MATH = 1 << 5,
		// the table manipulator and observer functions
		LUA_TABLE = 1 << 6,
		// the debug library
		LUA_DEBUG = 1 << 7,
		// the bit library: different based on which you're using
		LUA_BIT32 = 1 << 8,
		// input/output library
		LUA_IO = 1 << 9,
		// LuaJIT only
		LUA_FFI = 1 << 10,
		// LuaJIT only
		LUA_JIT = 1 << 11,
		// library for handling utf8: new to Lua
		LUA_UTF8 = 1 << 12,

		// all of the above
		LUA = LUA_BASE | LUA_PACKAGE | LUA_COROUTINE | LUA_STRING | LUA_OS | LUA_MATH | LUA_TABLE | LUA_DEBUG | LUA_BIT32 | LUA_IO | LUA_FFI | LUA_JIT | LUA_UTF8,

		// ----- Godot ----
		GODOT_VARIANT = 1 << 13,
		GODOT_UTILITY_FUNCTIONS = 1 << 14,
		GODOT_SINGLETONS = 1 << 15,
		GODOT_CLASSES = 1 << 16,
		GODOT_ENUMS = 1 << 17,

		// all of the above
		GODOT = GODOT_VARIANT | GODOT_UTILITY_FUNCTIONS | GODOT_SINGLETONS | GODOT_CLASSES | GODOT_ENUMS,
	};

	LuaState();
	~LuaState();

	void open_libraries(BitField<Library> libraries);

	LuaTable *create_table(const Dictionary& initial_values = {});
	Variant do_string(const String& chunk, const String& chunkname = "");
	Variant do_file(const String& filename, int buffer_size = 1024);

	LuaTable *get_globals() const;
	LuaTable *get_registry() const;

	operator String() const;

	static LuaState *find_lua_state(lua_State *L);

protected:
	static void _bind_methods();

	String _to_string() const;

	sol::state lua_state;

private:
	static HashMap<lua_State *, LuaState *> valid_states;
};

}
VARIANT_BITFIELD_CAST(luagdextension::LuaState::Library);

#endif
