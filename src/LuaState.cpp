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
#include "LuaState.hpp"

#include "LuaFunction.hpp"
#include "LuaTable.hpp"
#include "LuaThread.hpp"
#include "luaopen/godot.hpp"
#include "utils/_G_metatable.hpp"
#include "utils/convert_godot_lua.hpp"
#include "utils/module_names.hpp"

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <luaconf.h>

#ifndef LUAJIT
	#define LUA_GDEXTENSION_PATH_DEFAULT LUA_PATH_DEFAULT
#else
	#define LUA_GDEXTENSION_PATH_DEFAULT LUA_PATH_DEFAULT LUA_PATH_SEP "res://addons/lua-gdextension/build/?.lua"
#endif

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

static int lua_panic_handler(lua_State *L) {
	return sol::default_at_panic(L);
}

#ifdef HAVE_LUA_WARN
static void lua_warn_handler(void *ud, const char *msg, int tocont) {
	LuaState *L = (LuaState *) ud;
	L->warn(msg, tocont);
}
#endif

LuaState::LuaState()
#ifdef LUAJIT  // LuaJIT needs its default allocator in x64 platforms
	: lua_state(lua_panic_handler)
#else
	: lua_state(lua_panic_handler, lua_alloc)
#endif
{
	setup_G_metatable(lua_state);
#ifdef HAVE_LUA_WARN
	lua_setwarnf(lua_state, lua_warn_handler, this);
#endif
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

#ifdef LUAJIT
	// LuaJIT should have "jit/?.lua" in its Lua path
	if (libraries.has_flag(LUA_PACKAGE)) {
		set_package_path(";;");
	}
#endif

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
		if (libraries.has_flag(GODOT_LOCAL_PATHS)) {
			lua_state.require(module_names::local_paths, &luaopen_godot_local_paths, false);
		}
	}

	lua_state.registry().set("_GDEXTENSION_OPEN_LIBS", lua_state.registry().get_or("_GDEXTENSION_OPEN_LIBS", 0) | libraries);
}

bool LuaState::are_libraries_opened(BitField<Library> libraries) const {
	int64_t currently_opened_libs = lua_state.registry().get_or("_GDEXTENSION_OPEN_LIBS", 0L);
	return (currently_opened_libs & libraries) == libraries;
}

Ref<LuaTable> LuaState::create_table(const Dictionary& initial_values) {
	return memnew(LuaTable(to_table(lua_state, initial_values)));
}

Ref<LuaFunction> LuaState::create_function(const Callable& callable) {
	ERR_FAIL_COND_V(!callable.is_valid(), nullptr);
	return memnew(LuaFunction(to_lua_function(lua_state, callable)));
}

Variant LuaState::load_buffer(const PackedByteArray& chunk, const String& chunkname, LoadMode mode, LuaTable *env) {
	return ::luagdextension::load_buffer(lua_state, chunk, chunkname, (sol::load_mode) mode, env);
}

Variant LuaState::load_string(const String& chunk, const String& chunkname, LuaTable *env) {
	return ::luagdextension::load_buffer(lua_state, chunk.to_utf8_buffer(), chunkname, sol::load_mode::text, env);
}

Variant LuaState::load_file(const String& filename, LoadMode mode, LuaTable *env) {
	return ::luagdextension::load_file(lua_state, filename, (sol::load_mode) mode, env);
}

Variant LuaState::do_buffer(const PackedByteArray& chunk, const String& chunkname, LoadMode mode, LuaTable *env) {
	return ::luagdextension::do_buffer(lua_state, chunk, chunkname, (sol::load_mode) mode, env);
}

Variant LuaState::do_string(const String& chunk, const String& chunkname, LuaTable *env) {
	return ::luagdextension::do_buffer(lua_state, chunk.to_utf8_buffer(), chunkname, sol::load_mode::text, env);
}

Variant LuaState::do_file(const String& filename, LoadMode mode, LuaTable *env) {
	return ::luagdextension::do_file(lua_state, filename, (sol::load_mode) mode, env);
}

Ref<LuaTable> LuaState::get_globals() const {
	return LuaObject::wrap_object<LuaTable>(lua_state.globals());
}

Ref<LuaTable> LuaState::get_registry() const {
	return LuaObject::wrap_object<LuaTable>(lua_state.registry());
}

Ref<LuaThread> LuaState::get_main_thread() const {
	return LuaObject::wrap_object<LuaThread>(sol::thread(lua_state, lua_state));
}

String LuaState::get_package_path() const {
	if (auto package = lua_state.get<sol::optional<sol::table>>("package")) {
		return package->get<String>("path");
	}
	else {
		ERR_FAIL_V_MSG("", "LUA_PACKAGE library is not opened");
	}
}

String LuaState::get_package_cpath() const {
	if (auto package = lua_state.get<sol::optional<sol::table>>("package")) {
		return package->get<String>("cpath");
	}
	else {
		ERR_FAIL_V_MSG("", "LUA_PACKAGE library is not opened");
	}
}

void LuaState::set_package_path(const String& path) {
	if (auto package = lua_state.get<sol::optional<sol::table>>("package")) {
		package->set("path",
			path.replace(";;", LUA_PATH_SEP LUA_GDEXTENSION_PATH_DEFAULT LUA_PATH_SEP)
				.rstrip(LUA_PATH_SEP)
				.lstrip(LUA_PATH_SEP)
				.replace(LUA_EXEC_DIR, get_lua_exec_dir())
		);
	}
	else {
		ERR_FAIL_MSG("LUA_PACKAGE library is not opened");
	}
}

void LuaState::set_package_cpath(const String& cpath) {
	if (auto package = lua_state.get<sol::optional<sol::table>>("package")) {
		package->set("cpath",
			cpath.replace(";;", LUA_PATH_SEP LUA_CPATH_DEFAULT LUA_PATH_SEP)
				.rstrip(LUA_PATH_SEP)
				.lstrip(LUA_PATH_SEP)
				.replace(LUA_EXEC_DIR, get_lua_exec_dir())
		);
	}
	else {
		ERR_FAIL_MSG("LUA_PACKAGE library is not opened");
	}
}

String LuaState::get_lua_runtime() {
#ifdef LUAJIT
	return "luajit";
#else
	return "lua";
#endif
}

int LuaState::get_lua_version_num() {
	return LUA_VERSION_NUM;
}

String LuaState::get_lua_version_string() {
	return LUA_VERSION;
}

#ifdef HAVE_LUA_WARN
void LuaState::warn(const char *msg, int tocont) {
	if (msg[0] == '@') {
		if (msg == String("@off")) {
			warning_on = false;
		}
		else if (msg == String("@on")) {
			warning_on = true;
		}
	}
	else if (warning_on) {
		warn_message += msg;
		if (!tocont) {
			UtilityFunctions::push_warning(warn_message);
			warn_message = String();
		}
	}
}
#endif

String LuaState::get_lua_exec_dir() {
	return Engine::get_singleton()->is_editor_hint()
		? ProjectSettings::get_singleton()->globalize_path("res://")
		: OS::get_singleton()->get_executable_path().get_base_dir();
}

LuaState *LuaState::find_lua_state(lua_State *L) {
	L = sol::main_thread(L, L);
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
	BIND_BITFIELD_FLAG(GODOT_LOCAL_PATHS);
	BIND_BITFIELD_FLAG(GODOT_ALL_LIBS);
	BIND_BITFIELD_FLAG(ALL_LIBS);

	// LoadMode enum
	BIND_ENUM_CONSTANT(LOAD_MODE_ANY);
	BIND_ENUM_CONSTANT(LOAD_MODE_TEXT);
	BIND_ENUM_CONSTANT(LOAD_MODE_BINARY);

	// Methods
	ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuaState::open_libraries, DEFVAL(BitField<Library>(ALL_LIBS)));
	ClassDB::bind_method(D_METHOD("are_libraries_opened", "libraries"), &LuaState::are_libraries_opened);
	ClassDB::bind_method(D_METHOD("create_table", "initial_values"), &LuaState::create_table, DEFVAL(Dictionary()));
	ClassDB::bind_method(D_METHOD("create_function", "callable"), &LuaState::create_function);
	ClassDB::bind_method(D_METHOD("load_buffer", "chunk", "chunkname", "mode", "env"), &LuaState::load_buffer, DEFVAL(""), DEFVAL(LOAD_MODE_ANY), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("load_string", "chunk", "chunkname", "env"), &LuaState::load_string, DEFVAL(""), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("load_file", "filename", "mode", "env"), &LuaState::load_file, DEFVAL(LOAD_MODE_ANY), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("do_buffer", "chunk", "chunkname", "mode", "env"), &LuaState::do_buffer, DEFVAL(""), DEFVAL(LOAD_MODE_ANY), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("do_string", "chunk", "chunkname", "env"), &LuaState::do_string, DEFVAL(""), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("do_file", "filename", "mode", "env"), &LuaState::do_file, DEFVAL(LOAD_MODE_ANY), DEFVAL(nullptr));
	ClassDB::bind_method(D_METHOD("get_globals"), &LuaState::get_globals);
	ClassDB::bind_method(D_METHOD("get_registry"), &LuaState::get_registry);
	ClassDB::bind_method(D_METHOD("get_main_thread"), &LuaState::get_main_thread);
	ClassDB::bind_method(D_METHOD("get_package_path"), &LuaState::get_package_path);
	ClassDB::bind_method(D_METHOD("get_package_cpath"), &LuaState::get_package_cpath);
	ClassDB::bind_method(D_METHOD("set_package_path", "path"), &LuaState::set_package_path);
	ClassDB::bind_method(D_METHOD("set_package_cpath", "cpath"), &LuaState::set_package_cpath);
	ClassDB::bind_static_method(LuaState::get_class_static(), D_METHOD("get_lua_runtime"), &LuaState::get_lua_runtime);
	ClassDB::bind_static_method(LuaState::get_class_static(), D_METHOD("get_lua_version_num"), &LuaState::get_lua_version_num);
	ClassDB::bind_static_method(LuaState::get_class_static(), D_METHOD("get_lua_version_string"), &LuaState::get_lua_version_string);
	ClassDB::bind_static_method(LuaState::get_class_static(), D_METHOD("get_lua_exec_dir"), &LuaState::get_lua_exec_dir);

	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "globals", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE, LuaTable::get_class_static()), "", "get_globals");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "registry", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE, LuaTable::get_class_static()), "", "get_registry");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "main_thread", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE, LuaThread::get_class_static()), "", "get_main_thread");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "package_path", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_package_path", "get_package_path");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "package_cpath", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_package_cpath", "get_package_cpath");
}

LuaState::operator String() const {
	return _to_string();
}

String LuaState::_to_string() const {
	return String("[LuaState:0x%x]") % (int64_t) lua_state.lua_state();
}

HashMap<lua_State *, LuaState *> LuaState::valid_states;

}
