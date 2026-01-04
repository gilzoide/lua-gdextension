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

#include "../LuaTable.hpp"
#include "../generated/package_searcher.h"
#include "../utils/convert_godot_lua.hpp"
#include "../utils/load_fileaccess.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <luaconf.h>

using namespace luagdextension;

static int l_searchpath(lua_State *L) {
	String name = luaL_checkstring(L, 1);
	String path = luaL_checkstring(L, 2);
	String sep = luaL_optstring(L, 3, ".");
	String rep = luaL_optstring(L, 4, "/");
	if (!sep.is_empty()) {
		name = name.replace(sep, rep);
	}
	
	PackedStringArray path_list = path.split(LUA_PATH_SEP, false);
	PackedStringArray not_found_list;
	for (const String& path_template : path_list) {
		String filename = path_template.replace(LUA_PATH_MARK, name);
		if (FileAccess::file_exists(filename)) {
			sol::stack::push(L, filename);
			return 1;
		}
		else {
			not_found_list.append(filename);
		}
	}

	// path not found, return a formatted error message
	String error_message;
	for (const String& filename : not_found_list) {
		error_message += String("\n\tno file \"%s\"") % filename;
	}
	sol::stack::push(L, sol::nil);
	sol::stack::push(L, error_message);
	return 2;
}

static int l_loadfile(lua_State *L) {
	sol::state_view state(L);
	String filename = luaL_optstring(L, 1, "");
	String mode = luaL_optstring(L, 2, "bt");
	Variant env = to_variant(sol::stack_object(L, 3));

	sol::load_result result = load_fileaccess(state, filename, mode, Object::cast_to<LuaTable>(env));
	if (result.valid()) {
		result = sol::load_result();  // avoid popping result
		return 1;
	}
	else {
		lua_pushnil(L);
		lua_rotate(L, result.stack_index(), 1);
		result = sol::load_result();  // avoid popping error
		return 2;
	}
}

static int l_dofile(lua_State *L) {
	sol::state_view state(L);
	String filename = luaL_optstring(L, 1, "");
	
	int previous_top = lua_gettop(L);
	sol::load_result result = load_fileaccess(state, filename);
	if (result.valid()) {
		result = sol::load_result();  // avoid over-popping function
		lua_call(L, 0, LUA_MULTRET);
		return lua_gettop(L) - previous_top;
	}
	else {
		result = sol::load_result();  // avoid popping error
		return lua_error(L);
	}
}

extern "C" int luaopen_godot_local_paths(lua_State *L) {
	sol::state_view state = L;

	if (auto package = state.get<sol::optional<sol::table>>("package")) {
		(*package)["searchpath"] = l_searchpath;
		state.do_string(package_searcher_lua);
	}

	state["loadfile"] = l_loadfile;
	state["dofile"] = l_dofile;

	return 0;
}
 
 