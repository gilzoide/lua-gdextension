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
#include <string_view>

#include "LuaState.hpp"
#include "LuaError.hpp"
#include "godot_utils.hpp"
#include "lua_utils.hpp"

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/file_access.hpp>

namespace luagdextension {

LuaState::LuaState() : lua_state(sol::default_at_panic, lua_alloc) {
	table = lua_state.globals();
}

void LuaState::_bind_methods() {
	// Library enum
	BIND_BITFIELD_FLAG(LUA_BASE);
	BIND_BITFIELD_FLAG(LUA_PACKAGE);
	BIND_BITFIELD_FLAG(LUA_COROUTINE);
	BIND_BITFIELD_FLAG(LUA_STRING);
	BIND_BITFIELD_FLAG(LUA_OS);
	BIND_BITFIELD_FLAG(LUA_MATH);
	BIND_BITFIELD_FLAG(LUA_TABLE);
	BIND_BITFIELD_FLAG(LUA_DEBUG);
	BIND_BITFIELD_FLAG(LUA_BIT32);
	BIND_BITFIELD_FLAG(LUA_IO);
	BIND_BITFIELD_FLAG(LUA_FFI);
	BIND_BITFIELD_FLAG(LUA_JIT);
	BIND_BITFIELD_FLAG(LUA_UTF8);
	BIND_BITFIELD_FLAG(LUA);

	// Methods
	ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuaState::open_libraries, DEFVAL(BitField<Library>(LUA)));
	ClassDB::bind_method(D_METHOD("do_string", "chunk", "chunkname"), &LuaState::do_string, DEFVAL(""));
	ClassDB::bind_method(D_METHOD("do_file", "filename", "buffer_size"), &LuaState::do_file, DEFVAL(1024));
	ClassDB::bind_method(D_METHOD("get_globals"), &LuaState::get_globals);
	ClassDB::bind_method(D_METHOD("get_registry"), &LuaState::get_registry);
}

void LuaState::open_libraries(BitField<Library> libraries) {
	if (libraries == 0) {
		libraries = LUA;
	}

	if (libraries.has_flag(LUA)) {
		lua_state.open_libraries();
	}
	else {
		if (libraries.has_flag(LUA_BASE)) {
			lua_state.open_libraries(sol::lib::base);
		}
		if (libraries.has_flag(LUA_PACKAGE)) {
			lua_state.open_libraries(sol::lib::package);
		}
		if (libraries.has_flag(LUA_COROUTINE)) {
			lua_state.open_libraries(sol::lib::coroutine);
		}
		if (libraries.has_flag(LUA_STRING)) {
			lua_state.open_libraries(sol::lib::string);
		}
		if (libraries.has_flag(LUA_OS)) {
			lua_state.open_libraries(sol::lib::os);
		}
		if (libraries.has_flag(LUA_MATH)) {
			lua_state.open_libraries(sol::lib::math);
		}
		if (libraries.has_flag(LUA_TABLE)) {
			lua_state.open_libraries(sol::lib::table);
		}
		if (libraries.has_flag(LUA_DEBUG)) {
			lua_state.open_libraries(sol::lib::debug);
		}
		if (libraries.has_flag(LUA_BIT32)) {
			lua_state.open_libraries(sol::lib::bit32);
		}
		if (libraries.has_flag(LUA_IO)) {
			lua_state.open_libraries(sol::lib::io);
		}
		if (libraries.has_flag(LUA_FFI)) {
			lua_state.open_libraries(sol::lib::ffi);
		}
		if (libraries.has_flag(LUA_JIT)) {
			lua_state.open_libraries(sol::lib::jit);
		}
		if (libraries.has_flag(LUA_UTF8)) {
			lua_state.open_libraries(sol::lib::utf8);
		}
	}
}

Variant LuaState::do_string(const String& chunk, const String& chunkname) {
	return ::luagdextension::do_string(lua_state, chunk, chunkname);
}

Variant LuaState::do_file(const String& filename, int buffer_size) {
	return ::luagdextension::do_file(lua_state, filename);
}

LuaTable *LuaState::get_globals() const {
	return memnew(LuaTable(table));
}

LuaTable *LuaState::get_registry() const {
	return memnew(LuaTable(lua_state.registry()));
}

String LuaState::_to_string() const {
	return String("[LuaState:0x%x]") % (int64_t) lua_state.lua_state();
}

}
