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
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/script.hpp>

#include "LuaScript.hpp"
#include "LuaScriptLanguage.hpp"
#include "LuaScriptResourceFormatSaver.hpp"
#include "../LuaTable.hpp"

using namespace godot;

namespace luagdextension {

static Ref<LuaScriptResourceFormatSaver> lua_saver;

Error LuaScriptResourceFormatSaver::_save(const Ref<Resource> &resource, const String &p_path, uint32_t flags) {
	Ref<LuaScript> lua_script = resource;
	ERR_FAIL_COND_V(lua_script.is_null(), ERR_INVALID_PARAMETER);

	Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::ModeFlags::WRITE);
	ERR_FAIL_COND_V_MSG(file.is_null(), FileAccess::get_open_error(), "Failed to save file at " + p_path);

	file->store_string(lua_script->get_source_code());
	Error error = file->get_error();
	switch (error) {
		case OK:
		case ERR_FILE_EOF:
			return OK;
		
		default:
			return error;
	}
}

bool LuaScriptResourceFormatSaver::_recognize(const Ref<Resource> &resource) const {
	Ref<LuaScript> lua_script = resource;
	return lua_script.is_valid();
}

PackedStringArray LuaScriptResourceFormatSaver::_get_recognized_extensions(const Ref<Resource> &resource) const {
	PackedStringArray extensions;
	if (_recognize(resource)) {
		extensions.push_back("lua");
	}
	return extensions;
}

void LuaScriptResourceFormatSaver::register_in_godot() {
	lua_saver.instantiate();
	ResourceSaver::get_singleton()->add_resource_format_saver(lua_saver);
}

void LuaScriptResourceFormatSaver::unregister_in_godot() {
	ResourceSaver::get_singleton()->remove_resource_format_saver(lua_saver);
	lua_saver.unref();
}

void LuaScriptResourceFormatSaver::_bind_methods() {
}

}
