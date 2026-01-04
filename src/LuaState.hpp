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
#ifndef __LUA_STATE_HPP__
#define __LUA_STATE_HPP__

#include "utils/custom_sol.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace luagdextension {

class LuaFunction;
class LuaTable;
class LuaThread;

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
		LUA_ALL_LIBS = LUA_BASE | LUA_PACKAGE | LUA_COROUTINE | LUA_STRING | LUA_OS | LUA_MATH | LUA_TABLE | LUA_DEBUG | LUA_BIT32 | LUA_IO | LUA_FFI | LUA_JIT | LUA_UTF8,

		// ----- Godot ----
		// Variant types + String methods
		GODOT_VARIANT = 1 << 13,
		// utility functions, like "is_same" and "deg_to_rad". Also sets "print" to Godot's "printt"
		GODOT_UTILITY_FUNCTIONS = 1 << 14,
		// allows acessing singleton instances in _G
		GODOT_SINGLETONS = 1 << 15,
		// allows accessing classes by name in _G
		GODOT_CLASSES = 1 << 16,
		// global enums, like "KEY_A" and "PROPERTY_HINT_NONE"
		GODOT_ENUMS = 1 << 17,
		// add support for "res://" and "user://" paths in package searchers, "loadfile" and "dofile"
		GODOT_LOCAL_PATHS = 1 << 18,

		// all of the above
		GODOT_ALL_LIBS = GODOT_VARIANT | GODOT_UTILITY_FUNCTIONS | GODOT_SINGLETONS | GODOT_CLASSES | GODOT_ENUMS | GODOT_LOCAL_PATHS,

		ALL_LIBS = LUA_ALL_LIBS | GODOT_ALL_LIBS,
	};

	enum LoadMode {
		LOAD_MODE_ANY = (int) sol::load_mode::any,
		LOAD_MODE_TEXT = (int) sol::load_mode::text,
		LOAD_MODE_BINARY = (int) sol::load_mode::binary,
	};

	LuaState();
	virtual ~LuaState();

	sol::state_view get_lua_state() const;

	void open_libraries(BitField<Library> libraries = ALL_LIBS);
	bool are_libraries_opened(BitField<Library> libraries) const;

	Ref<LuaTable> create_table(const Dictionary& initial_values = {});
	Ref<LuaFunction> create_function(const Callable& callable);
	Variant load_buffer(const PackedByteArray& chunk, const String& chunkname = "", LoadMode mode = LOAD_MODE_ANY, LuaTable *env = nullptr);
	Variant load_string(const String& chunk, const String& chunkname = "", LuaTable *env = nullptr);
	Variant load_file(const String& filename, LoadMode mode = LOAD_MODE_ANY, LuaTable *env = nullptr);
	Variant do_buffer(const PackedByteArray& chunk, const String& chunkname = "", LoadMode mode = LOAD_MODE_ANY, LuaTable *env = nullptr);
	Variant do_string(const String& chunk, const String& chunkname = "", LuaTable *env = nullptr);
	Variant do_file(const String& filename, LoadMode mode = LOAD_MODE_ANY, LuaTable *env = nullptr);

	Ref<LuaTable> get_globals() const;
	Ref<LuaTable> get_registry() const;
	Ref<LuaThread> get_main_thread() const;

	String get_package_path() const;
	String get_package_cpath() const;
	void set_package_path(const String& path);
	void set_package_cpath(const String& cpath);

#ifdef HAVE_LUA_WARN
	void warn(const char *msg, int tocont);
#endif

	operator String() const;

	static String get_lua_runtime();
	static int get_lua_version_num();
	static String get_lua_version_string();

	static String get_lua_exec_dir();
	static LuaState *find_lua_state(lua_State *L);

protected:
	static void _bind_methods();

	String _to_string() const;

	sol::state lua_state;
#ifdef HAVE_LUA_WARN
	bool warning_on = true;
	String warn_message;
#endif

private:
	static HashMap<lua_State *, LuaState *> valid_states;
};

}
VARIANT_BITFIELD_CAST(luagdextension::LuaState::Library);
VARIANT_ENUM_CAST(luagdextension::LuaState::LoadMode);

#endif
