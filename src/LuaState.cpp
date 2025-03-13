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
#include "LuaState.hpp"

#include "LuaTable.hpp"
#include "luaopen/godot.hpp"
#include "utils/_G_metatable.hpp"
#include "utils/convert_godot_lua.hpp"
#include "utils/module_names.hpp"

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/file_access.hpp>

namespace luagdextension {

/// Lua memory allocation callback.
/// Uses Godot memory functions.
static void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	if (nsize == 0) {
		if (ptr != nullptr) {
			memfree(ptr);
		}
		return nullptr;
	}
	else {
		return memrealloc(ptr, nsize);
	}
}

LuaState::LuaState() : lua_state(sol::default_at_panic, lua_alloc) {
	setup_G_metatable(lua_state);
	valid_states.insert(lua_state, this);
}

LuaState::~LuaState() {
	valid_states.erase(lua_state);
}

sol::state_view LuaState::get_lua_state() const {
	return lua_state;
}

void LuaState::open_libraries(BitField<Library> libraries) {
	if ((libraries & LUA_ALL_LIBS) == LUA_ALL_LIBS) {
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

	if ((libraries & GODOT_ALL_LIBS) == GODOT_ALL_LIBS) {
		lua_state.require(module_names::godot, &luaopen_godot, false);
	}
	else {
		if (libraries.has_flag(GODOT_VARIANT)) {
			lua_state.require(module_names::variant, &luaopen_godot_variant, false);
		}
		if (libraries.has_flag(GODOT_UTILITY_FUNCTIONS)) {
			lua_state.require(module_names::utility_functions, &luaopen_godot_utility_functions, false);
		}
		if (libraries.has_flag(GODOT_SINGLETONS)) {
			lua_state.require(module_names::singleton_access, &luaopen_godot_singleton_access, false);
		}
		if (libraries.has_flag(GODOT_CLASSES)) {
			lua_state.require(module_names::classes, &luaopen_godot_classes, false);
		}
		if (libraries.has_flag(GODOT_ENUMS)) {
			lua_state.require(module_names::enums, &luaopen_godot_enums, false);
		}
	}
}

Ref<LuaTable> LuaState::create_table(const Dictionary& initial_values) {
	return memnew(LuaTable(to_table(lua_state, initial_values)));
}

Variant LuaState::load_string(const String& chunk, const String& chunkname, LuaTable *env) {
	return ::luagdextension::load_string(lua_state, chunk, chunkname, env);
}

Variant LuaState::load_file(const String& filename, int buffer_size, LuaTable *env) {
	return ::luagdextension::load_file(lua_state, filename, buffer_size, env);
}

Variant LuaState::do_string(const String& chunk, const String& chunkname, LuaTable *env) {
	return ::luagdextension::do_string(lua_state, chunk, chunkname, env);
}

Variant LuaState::do_file(const String& filename, int buffer_size, LuaTable *env) {
	return ::luagdextension::do_file(lua_state, filename, buffer_size, env);
}

LuaTable *LuaState::get_globals() const {
	return memnew(LuaTable(lua_state.globals()));
}

LuaTable *LuaState::get_registry() const {
	return memnew(LuaTable(lua_state.registry()));
}

LuaState *LuaState::find_lua_state(lua_State *L) {
	L = sol::main_thread(L);
	if (LuaState **ptr = valid_states.getptr(L)) {
		return *ptr;
	}
	else {
		return nullptr;
	}
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
	BIND_BITFIELD_FLAG(LUA_ALL_LIBS);

	BIND_BITFIELD_FLAG(GODOT_VARIANT);
	BIND_BITFIELD_FLAG(GODOT_UTILITY_FUNCTIONS);
	BIND_BITFIELD_FLAG(GODOT_SINGLETONS);
	BIND_BITFIELD_FLAG(GODOT_CLASSES);
	BIND_BITFIELD_FLAG(GODOT_ENUMS);
	BIND_BITFIELD_FLAG(GODOT_ALL_LIBS);
	
	BIND_BITFIELD_FLAG(ALL_LIBS);

	// Methods
	ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuaState::open_libraries, DEFVAL(BitField<Library>(ALL_LIBS)));
	ClassDB::bind_method(D_METHOD("create_table", "initial_values"), &LuaState::create_table, DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("load_string", "chunk", "chunkname", "env"), &LuaState::load_string, DEFVAL(""), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("load_file", "filename", "buffer_size", "env"), &LuaState::load_file, DEFVAL(1024), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("do_string", "chunk", "chunkname", "env"), &LuaState::do_string, DEFVAL(""), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("do_file", "filename", "buffer_size", "env"), &LuaState::do_file, DEFVAL(1024), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_globals"), &LuaState::get_globals);
	ClassDB::bind_method(D_METHOD("get_registry"), &LuaState::get_registry);

	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "globals", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE, LuaTable::get_class_static()), "", "get_globals");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "registry", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE, LuaTable::get_class_static()), "", "get_registry");
}

LuaState::operator String() const {
	return _to_string();
}

String LuaState::_to_string() const {
	return String("[LuaState:0x%x]") % (int64_t) lua_state.lua_state();
}

HashMap<lua_State *, LuaState *> LuaState::valid_states;

}
