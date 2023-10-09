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

#include "../LuaCoroutine.hpp"
#include "../LuaError.hpp"
#include "../LuaFunction.hpp"
#include "../LuaLightUserdata.hpp"
#include "../LuaTable.hpp"
#include "../LuaUserdata.hpp"
#include "DictionaryIterator.hpp"
#include "VariantArguments.hpp"
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
				return memnew(LuaUserdata(object.template as<sol::userdata>()));
			}
		}

		case sol::type::thread:
			return memnew(LuaCoroutine(object.template as<sol::thread>()));

		case sol::type::function:
			return memnew(LuaFunction(object.template as<sol::protected_function>()));

		case sol::type::lightuserdata:
			return memnew(LuaLightUserdata(object.template as<sol::lightuserdata>()));

		case sol::type::none:
		case sol::type::nil:
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

Variant to_variant(lua_State *L, int index) {
	return to_variant(sol::stack_object(L, index));
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

Array to_array(const sol::variadic_args& args) {
	Array arr;
	for (auto it : args) {
		arr.append(to_variant(it.get<sol::object>()));
	}
	return arr;
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

sol::table to_table(sol::state_view& state, const Dictionary& dictionary) {
	sol::table table = state.create_table();
	if (!dictionary.is_empty()) {
		DictionaryIterator iterator(dictionary);
		while (auto kvp = iterator.iter_next()) {
			Variant key, value;
			std::tie(key, value) = kvp.value();
			table[to_lua(state, key)] = to_lua(state, value);
		}
	}
	return table;
}

sol::stack_object variant_static_call_string_name(sol::this_state state, Variant::Type type, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	Variant::call_static(type, method, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String message = String("Invalid static call to method '{0}' in type {1}").format(Array::make(method, Variant::get_type_name(type)));
		lua_error(args.lua_state(), error, message);
	}
	return to_lua(state, result);
}
sol::stack_object variant_call_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	variant.call(method, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String message = String("Invalid call to method '{0}' in object of type {1}").format(Array::make(method, get_type_name(variant)));
		lua_error(args.lua_state(), error, message);
	}
	return to_lua(state, result);
}
sol::stack_object variant_call(sol::this_state state, Variant& variant, const char *method, const sol::variadic_args& args) {
	return variant_call_string_name(state, variant, method, args);
}

std::tuple<bool, sol::object> variant_pcall_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	variant.call(method, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error == GDEXTENSION_CALL_OK) {
		return std::make_tuple(true, to_lua(state, result));
	}
	else {
		return std::make_tuple(false, to_lua(state, to_string(error)));
	}
}
std::tuple<bool, sol::object> variant_pcall(sol::this_state state, Variant& variant, const char *method, const sol::variadic_args& args) {
	return variant_pcall_string_name(state, variant, method, args);
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

void lua_error(lua_State *L, const GDExtensionCallError& call_error, const String& prefix_message) {
	CharString prefix = prefix_message.ascii();
	CharString error_str = to_string(call_error).ascii();
	luaL_error(L, "%s: %s", prefix.get_data(), error_str.get_data());
}

}
