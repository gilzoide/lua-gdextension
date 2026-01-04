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
#include "load_fileaccess.hpp"
#include "convert_godot_lua.hpp"
#include "convert_godot_std.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

struct FileReaderData {
	FileAccess *file;
	size_t buffer_size;
	PackedByteArray bytes;
};
static const char *file_reader(lua_State *L, FileReaderData *data, size_t *size) {
	if (data->file->eof_reached()) {
		*size = 0;
		return nullptr;
	}
	else {
		data->bytes = data->file->get_buffer(data->buffer_size);
		*size = data->bytes.size();
		return (const char *) data->bytes.ptr();
	}
}

sol::load_result load_fileaccess(sol::state_view& lua_state, const String& filename, sol::load_mode mode, LuaTable *env) {
	if (filename.is_empty()) {
		int lua_result = luaL_loadfilex(lua_state, nullptr, sol::to_string(mode).c_str());
		return sol::load_result(lua_state, lua_absindex(lua_state, -1), 1, 1, (sol::load_status) lua_result); 
	}

	auto file = FileAccess::open(filename, godot::FileAccess::READ);
	if (file == nullptr) {
		lua_push(lua_state, String("Cannot open file '%s': %s") % Array::make(filename, UtilityFunctions::error_string(FileAccess::get_open_error())));
		return sol::load_result(lua_state, lua_absindex(lua_state, -1), 1, 1, sol::load_status::file);
	}

	FileReaderData reader_data;
	reader_data.file = file.ptr();
	reader_data.buffer_size = 1024;
	sol::load_result result = lua_state.load((lua_Reader) file_reader, (void *) &reader_data, to_std_string(filename), mode);
	if (result.valid() && env) {
		lua_push(lua_state, (const Object *) env);
#if LUA_VERSION_NUM >= 502
		lua_setupvalue(lua_state, result.stack_index(), 1);
#else
		lua_setfenv(lua_state, result.stack_index());
#endif
	}
	return result;
}

sol::load_result load_fileaccess(sol::state_view& lua_state, const String& filename, String mode_str, LuaTable *env) {
	bool has_b = mode_str.contains("b");
	bool has_t = mode_str.contains("t");
	sol::load_mode mode;
	if (has_b && !has_t) {
		mode = sol::load_mode::binary;
	}
	else if (has_t && !has_b) {
		mode = sol::load_mode::text;
	}
	else {
		mode = sol::load_mode::any;
	}
	return load_fileaccess(lua_state, filename, mode, env);
}

}
 