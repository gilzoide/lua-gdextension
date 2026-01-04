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
#include "project_settings.hpp"

#include <godot_cpp/classes/project_settings.hpp>

namespace luagdextension {

static void add_project_setting(ProjectSettings *project_settings, const String& setting_name, const Variant& initial_value, bool is_basic = false, bool is_internal = false) {
	if (!project_settings->has_setting(setting_name)) {
		project_settings->set_setting(setting_name, initial_value);
	}
	project_settings->set_initial_value(setting_name, initial_value);
	project_settings->set_as_basic(setting_name, is_basic);
	project_settings->set_as_internal(setting_name, is_internal);
}

void register_project_settings() {
	ProjectSettings *project_settings = ProjectSettings::get_singleton();
#ifndef LUAJIT
	add_project_setting(project_settings, LUA_PATH_SETTING, "res://?.lua;res://?/init.lua");
#else
	add_project_setting(project_settings, LUA_PATH_SETTING, "res://?.lua;res://?/init.lua;res://addons/lua-gdextension/build/?.lua");
#endif
	add_project_setting(project_settings, LUA_CPATH_SETTING, "!/?.so;!/loadall.so");
	add_project_setting(project_settings, LUA_CPATH_WINDOWS_SETTING, "!/?.dll;!/loadall.dll");
	add_project_setting(project_settings, LUA_CPATH_MACOS_SETTING, "!/?.dylib;!/loadall.dylib");
	add_project_setting(project_settings, LUA_SCRIPT_IMPORT_MAP_SETTING_EDITOR, Dictionary(), false, true);
}

}
