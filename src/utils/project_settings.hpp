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
#ifndef __UTILS_PROJECT_SETTINGS_HPP__
#define __UTILS_PROJECT_SETTINGS_HPP__

namespace godot {
	class ProjectSettings;
	class String;
	class Variant;
}
using namespace godot;


namespace luagdextension {

constexpr char LUA_PATH_SETTING[] = "lua_gdextension/lua_script_language/package_path";
constexpr char LUA_CPATH_SETTING[] = "lua_gdextension/lua_script_language/package_c_path";
constexpr char LUA_CPATH_WINDOWS_SETTING[] = "lua_gdextension/lua_script_language/package_c_path.windows";
constexpr char LUA_CPATH_MACOS_SETTING[] = "lua_gdextension/lua_script_language/package_c_path.macos";
constexpr char LUA_SCRIPT_IMPORT_MAP_SETTING[] = "lua_gdextension/lua_script_language/script_import_map";
constexpr char LUA_SCRIPT_IMPORT_MAP_SETTING_EDITOR[] = "lua_gdextension/lua_script_language/script_import_map.editor";

void register_project_settings();

}

#endif  // __UTILS_PROJECT_SETTINGS_HPP__
