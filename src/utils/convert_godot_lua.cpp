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
#include "convert_godot_lua.hpp"

#include "../LuaCoroutine.hpp"
#include "../LuaError.hpp"
#include "../LuaFunction.hpp"
#include "../LuaLightUserdata.hpp"
#include "../LuaTable.hpp"
#include "../LuaUserdata.hpp"
#include "../script-language/LuaScriptInstance.hpp"
#include "Class.hpp"
#include "DictionaryIterator.hpp"
#include "VariantArguments.hpp"
#include "convert_godot_std.hpp"
#include "extra_utility_functions.hpp"
#include "load_fileaccess.hpp"
#include "method_bind_impl.hpp"
#include "stack_top_checker.hpp"

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
			if (LuaScriptInstance *script_instance = LuaScriptInstance::find_instance(object.template as<sol::basic_table<ref_t>>())) {
				return script_instance->owner;
			}
			else {
				return LuaObject::wrap_object<LuaTable>(object);
			}

		case sol::type::userdata:
			if (object.template is<Variant>()) {
				GDExtensionVariantPtr variant_ptr = object.template as<Variant *>();
				return Variant(variant_ptr);
			}
			else if (object.template is<Class>()) {
				Class& cls = object.template as<Class&>();
				return cls.get_name();
			}
			else if (object.template is<VariantMethodBind>()) {
				VariantMethodBind& method_bind = object.template as<VariantMethodBind&>();
				return method_bind.to_callable();
			}
			else if (object.template is<LuaScriptInstanceMethodBind>()) {
				LuaScriptInstanceMethodBind& method_bind = object.template as<LuaScriptInstanceMethodBind&>();
				return method_bind.to_callable();
			}
			else {
				return LuaObject::wrap_object<LuaUserdata>(object);
			}

		case sol::type::thread: {
			sol::basic_thread<ref_t> thread(object);
			if (thread.is_main_thread()) {
				return LuaObject::wrap_object<LuaThread>(thread);
			}
			else {
				return LuaObject::wrap_object<LuaCoroutine>(thread);
			}
		}

		case sol::type::function:
			return LuaObject::wrap_object<LuaFunction>(object);

		case sol::type::lightuserdata:
			return LuaObject::wrap_object<LuaLightUserdata>(object);

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

Variant to_variant(const sol::protected_function_result& function_result, bool return_lua_error) {
	if (!function_result.valid()) {
		if (return_lua_error) {
			return memnew(LuaError(function_result));
		}
		else {
			ERR_PRINT(LuaError::extract_message(function_result));
			return Variant();
		}
	}

	switch (function_result.return_count()) {
		case 0:
			return Variant();

		case 1:
			return to_variant(function_result[0].get<sol::stack_object>());

		default:
			Array arr;
			for (auto value : function_result) {
				arr.append(to_variant(value.get<sol::stack_object>()));
			}
			return arr;
	}
}

Variant to_variant(const sol::load_result& load_result) {
	if (!load_result.valid()) {
		return memnew(LuaError(load_result));
	}
	else {
		return LuaObject::wrap_object<LuaFunction>(load_result.get<sol::protected_function>());
	}
}

Variant to_variant(lua_State *L, int index) {
	return to_variant(sol::stack_object(L, index));
}

sol::stack_object lua_push(lua_State *lua_state, const Variant& value) {
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

		case Variant::OBJECT:
			if (!is_instance_valid(value)) {
				sol::stack::push(lua_state, sol::nil);
				break;
			}
			if (LuaObject *lua_obj = Object::cast_to<LuaObject>(value)) {
				if (LuaState::find_lua_state(lua_state) == lua_obj->get_lua_state()) {
					sol::stack::push(lua_state, lua_obj->get_lua_object());
					break;
				}
			}
			if (LuaScriptInstance *script_instance = LuaScriptInstance::attached_to_object(value)) {
				if (LuaState::find_lua_state(lua_state) == script_instance->get_lua_state()) {
					sol::stack::push(lua_state, script_instance->data->get_table());
					break;
				}
			}
			goto push_as_variant;
			
		case Variant::CALLABLE:
			if (LuaState *gdlua = LuaState::find_lua_state(lua_state); gdlua->are_libraries_opened(LuaState::GODOT_VARIANT)) {
				goto push_as_variant;
			}
			else {
				// If GODOT_VARIANT library is not opened in this lua_State, push Callable wrapped in a Lua function
				lua_push_function(lua_state, (Callable) value);
				break;
			}

push_as_variant:
		default:
			sol::stack::push_userdata(lua_state, (Variant) value);
			break;
	}
	return sol::stack_object(lua_state, -1);
}

sol::object to_lua(lua_State *lua_state, const Variant& value) {
	sol::object result = lua_push(lua_state, value);
	lua_pop(lua_state, 1);
	return result;
}

void fill_array(Array& array, const sol::table& table) {
	for (int i = 1; i <= table.size(); i++) {
		array.append(to_variant(table.get<sol::object>(i)));
	}
}

void fill_array(Array& array, const sol::variadic_args& args) {
	for (auto it : args) {
		array.append(to_variant(it.get<sol::stack_object>()));
	}
}

void fill_dictionary(Dictionary& dict, const sol::table& table) {
	for (auto it : table.pairs()) {
		dict[to_variant(it.first)] = to_variant(it.second);
	}
}

Array to_array(const sol::table& table) {
	Array arr;
	fill_array(arr, table);
	return arr;
}

Dictionary to_dictionary(const sol::table& table) {
	Dictionary dict;
	fill_dictionary(dict, table);
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

static int callable_closure(lua_State *L) {
	Callable callable = to_variant(L, lua_upvalueindex(1));
	sol::variadic_args args(L, 1);
	Variant result = callable.callv(VariantArguments(args).get_array());
	lua_push(L, result);
	return 1;
}
void lua_push_function(lua_State *L, const Callable& callable) {
	ERR_FAIL_COND(!callable.is_valid());
	StackTopChecker topcheck(L, 1);
	lua_push_closure(L, callable_closure, (Variant) callable);
}

sol::protected_function to_lua_function(lua_State *L, const Callable& callable) {
	ERR_FAIL_COND_V(!callable.is_valid(), nullptr);
	StackTopChecker topcheck(L);
	return to_lua_closure(L, callable_closure, (Variant) callable);
}

Variant callable_call(const Callable& callable, const sol::variadic_args& args) {
	return callable.callv(VariantArguments(args).get_array());
}

sol::object variant_static_call_string_name(sol::this_state state, Variant::Type type, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	Variant::callp_static(type, method, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String message = String("Invalid static call to method '{0}' in type {1}").format(Array::make(method, Variant::get_type_name(type)));
		lua_error(state, error, message);
	}
	return to_lua(state, result);
}
sol::object variant_call_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	variant.callp(method, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String message = String("Invalid call to method '{0}' in object of type {1}").format(Array::make(method, get_type_name(variant)));
		lua_error(state, error, message);
	}
	return to_lua(state, result);
}
sol::object variant_call(sol::this_state state, Variant& variant, const char *method, const sol::variadic_args& args) {
	return variant_call_string_name(state, variant, method, args);
}

std::tuple<bool, sol::object> variant_pcall_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	variant.callp(method, variant_args.argv(), variant_args.argc(), result, error);
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

Variant do_buffer(sol::state_view& lua_state, const PackedByteArray& chunk, const String& chunkname, sol::load_mode mode, LuaTable *env) {
	Variant load_result = load_buffer(lua_state, chunk, chunkname, mode, env);
	if (LuaFunction *func = Object::cast_to<LuaFunction>(load_result)) {
		return func->invokev(Array());
	}
	else {
		return load_result;
	}
}

Variant do_file(sol::state_view& lua_state, const String& filename, sol::load_mode mode, LuaTable *env) {
	Variant load_result = load_file(lua_state, filename, mode, env);
	if (LuaFunction *func = Object::cast_to<LuaFunction>(load_result)) {
		return func->invokev(Array());
	}
	else {
		return load_result;
	}
}

Variant load_buffer(sol::state_view& lua_state, const PackedByteArray& chunk, const String& chunkname, sol::load_mode mode, LuaTable *env) {
	sol::load_result result = lua_state.load(to_string_view(chunk), to_std_string(chunkname), mode);
	if (result.valid() && env) {
		lua_push(lua_state, (const Object *) env);
#if LUA_VERSION_NUM >= 502
		lua_setupvalue(lua_state, result.stack_index(), 1);
#else
		lua_setfenv(lua_state, result.stack_index());
#endif
	}
	return to_variant(result);
}

Variant load_file(sol::state_view& lua_state, const String& filename, sol::load_mode mode, LuaTable *env) {
	return to_variant(load_fileaccess(lua_state, filename, mode, env));
}

void lua_error(lua_State *L, const GDExtensionCallError& call_error, const String& prefix_message) {
	CharString prefix = prefix_message.ascii();
	CharString error_str = to_string(call_error).ascii();
	luaL_error(L, "%s: %s", prefix.get_data(), error_str.get_data());
}

}
