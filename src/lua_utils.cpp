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

#include "LuaError.hpp"
#include "LuaTable.hpp"
#include "constants.hpp"
#include "godot_utils.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/file_access.hpp>
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

template<typename ref_t>
Variant to_variant(const sol::basic_object<ref_t>& object) {
	switch (object.get_type()) {
		case sol::type::boolean:
			return object.template as<bool>();

		case sol::type::string:
			return object.template as<String>();

		case sol::type::number:
#if LUA_VERSION_NUM >= 503
			if (sol::utility::is_integer(object)) {
				return object.template as<int64_t>();
			}
#endif
			return object.template as<double>();

		case sol::type::table:
			return memnew(LuaTable(object.template as<sol::table>()));

		case sol::type::userdata: {
			auto userdata = object.template as<sol::basic_userdata<ref_t>>();
			sol::optional<int> variant_type = userdata[LUA_META_VARIANT_TYPE];
			if (!variant_type.has_value()) {
				WARN_PRINT_ONCE_ED("Lua type 'full userdata' is not supported yet");
				return Variant();
			}
			
			switch (variant_type.value()) {
				case Variant::NIL:
				case Variant::BOOL:
				case Variant::INT:
				case Variant::FLOAT:
				case Variant::STRING:
				case Variant::STRING_NAME:
					ERR_FAIL_V_EDMSG(Variant(), "Found variant type for primitive values, this should not happen");

				case Variant::VECTOR2:
					return userdata.template as<Vector2>();

				case Variant::VECTOR2I:
					return userdata.template as<Vector2i>();

				case Variant::VECTOR3:
					return userdata.template as<Vector3>();

				case Variant::VECTOR3I:
					return userdata.template as<Vector3i>();

				case Variant::VECTOR4:
					return userdata.template as<Vector4>();

				case Variant::VECTOR4I:
					return userdata.template as<Vector4i>();

				case Variant::RECT2:
					return userdata.template as<Rect2>();

				case Variant::RECT2I:
					return userdata.template as<Rect2i>();

				case Variant::PLANE:
					return userdata.template as<Plane>();

				case Variant::TRANSFORM2D:
				case Variant::QUATERNION:
				case Variant::AABB:
				case Variant::BASIS:
				case Variant::TRANSFORM3D:
				case Variant::PROJECTION:
				case Variant::COLOR:
				case Variant::NODE_PATH:
				case Variant::RID:
				case Variant::OBJECT:
				case Variant::CALLABLE:
				case Variant::SIGNAL:
				case Variant::DICTIONARY:
				case Variant::ARRAY:
				case Variant::PACKED_BYTE_ARRAY:
				case Variant::PACKED_INT32_ARRAY:
				case Variant::PACKED_INT64_ARRAY:
				case Variant::PACKED_FLOAT32_ARRAY:
				case Variant::PACKED_FLOAT64_ARRAY:
				case Variant::PACKED_STRING_ARRAY:
				case Variant::PACKED_VECTOR2_ARRAY:
				case Variant::PACKED_VECTOR3_ARRAY:
				case Variant::PACKED_COLOR_ARRAY:
					ERR_FAIL_V_EDMSG(Variant(), "Found variant type that is not supported yet");
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

		default:
		case sol::type::none:
		case sol::type::lua_nil:
			return Variant();
	}
}

Variant to_variant(const sol::object& object) {
	return to_variant<>(object);
}

Variant to_variant(const sol::stack_object& object) {
	return to_variant<>(object);
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

		case Variant::VECTOR2:
			sol::stack::push(lua_state, (Vector2) value);
			break;

		case Variant::VECTOR2I:
			sol::stack::push(lua_state, (Vector2i) value);
			break;

		case Variant::VECTOR3:
			sol::stack::push(lua_state, (Vector3) value);
			break;

		case Variant::VECTOR3I:
			sol::stack::push(lua_state, (Vector3i) value);
			break;

		case Variant::VECTOR4:
			sol::stack::push(lua_state, (Vector4) value);
			break;

		case Variant::VECTOR4I:
			sol::stack::push(lua_state, (Vector4i) value);
			break;

		case Variant::RECT2:
			sol::stack::push(lua_state, (Rect2) value);
			break;

		case Variant::RECT2I:
			sol::stack::push(lua_state, (Rect2i) value);
			break;

		case Variant::PLANE:
			sol::stack::push(lua_state, (Plane) value);
			break;

		case Variant::TRANSFORM2D:
		case Variant::QUATERNION:
		case Variant::AABB:
		case Variant::BASIS:
		case Variant::TRANSFORM3D:
		case Variant::PROJECTION:
		case Variant::COLOR:
		case Variant::NODE_PATH:
		case Variant::RID:
		case Variant::OBJECT:
		case Variant::CALLABLE:
		case Variant::SIGNAL:
		case Variant::DICTIONARY:
		case Variant::ARRAY:
		case Variant::PACKED_BYTE_ARRAY:
		case Variant::PACKED_INT32_ARRAY:
		case Variant::PACKED_INT64_ARRAY:
		case Variant::PACKED_FLOAT32_ARRAY:
		case Variant::PACKED_FLOAT64_ARRAY:
		case Variant::PACKED_STRING_ARRAY:
		case Variant::PACKED_VECTOR2_ARRAY:
		case Variant::PACKED_VECTOR3_ARRAY:
		case Variant::PACKED_COLOR_ARRAY:
			WARN_PRINT_ONCE_ED("Lua type not yet supported");

		case Variant::NIL:
		default:
			sol::stack::push(lua_state, sol::nil);
			break;
	}
	return sol::stack_object(lua_state, -1);
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
		return memnew(LuaError(LuaError::Status::FILE, String("Cannot open file '%s': " + error_to_string(FileAccess::get_open_error())) % filename));
	}

	FileReaderData reader_data;
	reader_data.file = file.ptr();
	reader_data.buffer_size = buffer_size;
	return to_variant(lua_state.safe_script((lua_Reader) file_reader, (void *) &reader_data, sol::script_pass_on_error, to_std_string(filename)));
}

}
