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

#include "../utils/custom_sol.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>

static int searchpath(lua_State *L, const String& name, const String& path, const String& sep, const String& rep) {
	String template_name = sep.is_empty() ? name : name.replace(sep, rep);
	String execdir_repl = Engine::get_singleton()->is_editor_hint()
		? ProjectSettings::get_singleton()->globalize_path("res://")
		: OS::get_singleton()->get_executable_path().get_base_dir();
	
	PackedStringArray path_list = path.split(";", false);
	PackedStringArray not_found_list;
	for (const String& path_template : path_list) {
		String filename = path_template.replace("?", template_name).replace("!", execdir_repl);
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

static int l_searchpath(lua_State *L) {
	return searchpath(L, luaL_checkstring(L, 1), luaL_checkstring(L, 2), luaL_optstring(L, 3, "."), luaL_optstring(L, 4, "/"));
}

extern "C" int luaopen_godot_package_searcher(lua_State *L) {
	sol::state_view state = L;

	if (auto maybe_package = state.get<sol::optional<sol::table>>("package")) {
		sol::table package = *maybe_package;
		package["searchpath"] = l_searchpath;
	}

	return 0;
}
 
 