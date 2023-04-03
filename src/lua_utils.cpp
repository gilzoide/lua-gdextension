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
#include "lua_utils.hpp"
#include "LuaTable.hpp"
#include "godot_utils.hpp"
#include "LuaError.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>
#if LUA_VERSION_NUM >= 503
#include <sol/utility/is_integer.hpp>
#endif

using namespace godot;

namespace luagdextension {

void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
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

Variant to_variant(const sol::object& object) {
	switch (object.get_type()) {
		case sol::type::boolean:
			return object.as<bool>();

		case sol::type::string: {
			auto sv = object.as<std::string_view>();
			return String::utf8(sv.data(), sv.length());
		}

		case sol::type::number:
#if LUA_VERSION_NUM >= 503
			if (sol::utility::is_integer(object)) {
				return object.as<int64_t>();
			}
#endif
			return object.as<double>();

		case sol::type::table:
			return memnew(LuaTable(object.as<sol::table>()));

		case sol::type::thread:
		case sol::type::function:
		case sol::type::userdata:
		case sol::type::lightuserdata:
			WARN_PRINT_ONCE_ED("Lua type not yet supported");

		default:
		case sol::type::none:
		case sol::type::lua_nil:
			return Variant();
	}
}

Variant to_variant(const sol::stack_proxy_base& stack) {
	return to_variant(stack.get<sol::object>());
}

Variant to_variant(const sol::protected_function_result& function_result) {
	if (!function_result.valid()) {
		LuaError::Status status = (LuaError::Status) function_result.status();
		String message = ((sol::error) function_result).what();
		return memnew(LuaError(status, message));
	}

	switch (function_result.return_count()) {
		case 0:
			return Variant();

		case 1:
			return to_variant(function_result[0]);

		default:
			auto arr = Array();
			for (auto it = function_result.cbegin(); it != function_result.cend(); it++) {
				arr.append(to_variant(*it));
			}
			return arr;
	}
}

Variant do_string(sol::state_view& lua_state, const String& chunk, const String& chunkname) {
	CharString chunk_utf8 = chunk.utf8();
	return to_variant(lua_state.safe_script(to_string_view(chunk_utf8), sol::script_pass_on_error, to_std_string(chunkname)));
}

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

Variant do_file(sol::state_view& lua_state, const String& filename, int buffer_size) {
	auto file = FileAccess::open(filename, godot::FileAccess::READ);
	if (file == nullptr) {
		LuaError *error = memnew(LuaError);
		error->set_status(LuaError::Status::FILE);
		error->set_message(String("Cannot open file '%s': " + error_to_string(FileAccess::get_open_error())) % filename);
		return error;
	}

	FileReaderData reader_data;
	reader_data.file = file.ptr();
	reader_data.buffer_size = buffer_size;
	return to_variant(lua_state.safe_script((lua_Reader) file_reader, (void *) &reader_data, sol::script_pass_on_error, to_std_string(filename)));
}

}
