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
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/script.hpp>

#include "LuaScript.hpp"
#include "LuaScriptLanguage.hpp"
#include "LuaScriptResourceFormatLoader.hpp"

namespace luagdextension {

static Ref<LuaScriptResourceFormatLoader> lua_loader;

PackedStringArray LuaScriptResourceFormatLoader::_get_recognized_extensions() const {
	return Array::make("lua");
}

bool LuaScriptResourceFormatLoader::_handles_type(const StringName &p_type) const {
	return p_type == Script::get_class_static() || p_type == LuaScript::get_class_static();
}

String LuaScriptResourceFormatLoader::_get_resource_type(const String &p_path) const {
	if (p_path.get_extension() == "lua") {
		return LuaScript::get_class_static();
	}
	else {
		return "";
	}
}

bool LuaScriptResourceFormatLoader::_exists(const String &p_path) const {
	return FileAccess::file_exists(p_path);
}

Variant LuaScriptResourceFormatLoader::_load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const {
	Ref<LuaScript> script;
	script.instantiate();

	switch (p_cache_mode) {
		case ResourceFormatLoader::CACHE_MODE_IGNORE:
		case ResourceFormatLoader::CACHE_MODE_IGNORE_DEEP:
			break;

		case ResourceFormatLoader::CACHE_MODE_REUSE: {
			Ref<LuaScript> existing_script = ResourceLoader::get_singleton()->get_cached_ref(p_path);
			if (existing_script.is_null()) {
				script->set_path(p_original_path);
			}
			else {
				script = existing_script;
			}
			break;
		}

		case ResourceFormatLoader::CACHE_MODE_REPLACE:
		case ResourceFormatLoader::CACHE_MODE_REPLACE_DEEP:
			script->take_over_path(p_original_path);
			break;
	}

	script->set_source_code(FileAccess::get_file_as_string(p_path));
	Error status = script->reload();
	if (status == OK) {
		return script;
	}
	else {
		return status;
	}
}

void LuaScriptResourceFormatLoader::register_in_godot() {
	lua_loader.instantiate();
	ResourceLoader::get_singleton()->add_resource_format_loader(lua_loader);
}

void LuaScriptResourceFormatLoader::unregister_in_godot() {
	ResourceLoader::get_singleton()->remove_resource_format_loader(lua_loader);
	lua_loader.unref();
}

void LuaScriptResourceFormatLoader::_bind_methods() {
}

}
