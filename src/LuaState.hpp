#ifndef __LUA_STATE_HPP__
#define __LUA_STATE_HPP__

#include <godot_cpp/classes/ref_counted.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

class LuaState : public RefCounted {
	GDCLASS(LuaState, RefCounted);

private:
	sol::state lua_state;

protected:
	static void _bind_methods();

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

	static void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize);

	LuaState();
	void open_libraries(BitField<Library> libraries);
};

}
VARIANT_BITFIELD_CAST(luagdextension::LuaState::Library);

#endif
