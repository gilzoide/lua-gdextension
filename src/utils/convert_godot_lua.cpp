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
#include "convert_godot_lua.hpp"

#include "../LuaError.hpp"
#include "../LuaTable.hpp"
#include "convert_godot_std.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <sol/utility/is_integer.hpp>

using namespace godot;

namespace luagdextension {

template<typename ref_t>
Variant to_variant(const sol::basic_object<ref_t>& object) {
	switch (object.get_type()) {
		case sol::type::boolean:
			return object.template as<bool>();

		case sol::type::string:
			return object.template as<String>();

		case sol::type::number:
			if (sol::utility::is_integer(object)) {
				return object.template as<int64_t>();
			}
			return object.template as<double>();

		case sol::type::table:
			return memnew(LuaTable(object.template as<sol::table>()));

		case sol::type::userdata: {
			if (object.template is<Variant>()) {
				GDExtensionVariantPtr variant_ptr = object.template as<Variant *>();
				return Variant(variant_ptr);
			}
			else {
				WARN_PRINT_ONCE_ED("Lua type 'full userdata' is not supported yet");
				return Variant();
			}
		}

		case sol::type::thread:
			WARN_PRINT_ONCE_ED("Lua type 'thread' is not supported yet");
			return Variant();

		case sol::type::function:
			WARN_PRINT_ONCE_ED("Lua type 'function' is not supported yet");
			return Variant();

		case sol::type::lightuserdata:
			WARN_PRINT_ONCE_ED("Lua type 'light userdata' is not supported yet");
			return Variant();

		case sol::type::none:
		case sol::type::lua_nil:
		default:
			return Variant();
	}
}

Variant to_variant(const sol::object& object) {
	return to_variant<>(object);
}

Variant to_variant(const sol::stack_object& object) {
	return to_variant<>(object);
}

Variant to_variant(const sol::stack_proxy_base& proxy) {
	return to_variant<>(sol::stack_object(proxy.lua_state(), proxy.stack_index()));
}

Variant to_variant(const sol::protected_function_result& function_result) {
	if (!function_result.valid()) {
		return memnew(LuaError(function_result));
	}

	switch (function_result.return_count()) {
		case 0:
			return Variant();

		case 1:
			return to_variant(function_result[0].get<sol::stack_object>());

		default:
			auto arr = Array();
			for (auto value : function_result) {
				arr.append(to_variant(value.get<sol::stack_object>()));
			}
			return arr;
	}
}

sol::stack_object to_lua(lua_State *lua_state, const Variant& value) {
	switch (value.get_type()) {
		case Variant::NIL:
			sol::stack::push(lua_state, sol::nil);
			break;

		case Variant::BOOL:
			sol::stack::push(lua_state, (bool) value);
			break;

		case Variant::INT:
			sol::stack::push(lua_state, (int64_t) value);
			break;

		case Variant::FLOAT:
			sol::stack::push(lua_state, (double) value);
			break;

		case Variant::STRING:
			sol::stack::push(lua_state, (String) value);
			break;

		case Variant::STRING_NAME:
			sol::stack::push(lua_state, (StringName) value);
			break;

		default:
			sol::stack::push(lua_state, value);
			break;
	}
	return sol::stack_object(lua_state, -1);
}

Array to_array(const sol::table& table) {
	Array arr;
	for (int i = 1; i <= table.size(); i++) {
		arr.append(to_variant(table.get<sol::object>(i)));
	}
	return arr;
}

Dictionary to_dictionary(const sol::table& table) {
	Dictionary dict;
	for (auto it : table.pairs()) {
		dict[to_variant(it.first)] = to_variant(it.second);
	}
	return dict;
}

Variant do_string(sol::state_view& lua_state, const String& chunk, const String& chunkname) {
	PackedByteArray bytes = chunk.to_utf8_buffer();
	return to_variant(lua_state.safe_script(to_string_view(bytes), sol::script_pass_on_error, to_std_string(chunkname)));
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
		return memnew(LuaError(LuaError::Status::FILE, String("Cannot open file '%s': " + UtilityFunctions::error_string(FileAccess::get_open_error())) % filename));
	}

	FileReaderData reader_data;
	reader_data.file = file.ptr();
	reader_data.buffer_size = buffer_size;
	return to_variant(lua_state.safe_script((lua_Reader) file_reader, (void *) &reader_data, sol::script_pass_on_error, to_std_string(filename)));
}

}