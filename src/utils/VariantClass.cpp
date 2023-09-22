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
#include "VariantClass.hpp"

#include "MethodBindByName.hpp"
#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

VariantClass::VariantClass(Variant::Type type) : type(type) {}

Variant::Type VariantClass::get_type() const {
	return type;
}

String VariantClass::get_type_name() const {
	return Variant::get_type_name(type);
}

Variant VariantClass::construct(const sol::variadic_args& args) const {
	if (args.size() == 1 && args.get_type() == sol::type::table) {
		if (type == Variant::ARRAY) {
			return to_array(args.get<sol::stack_table>());
		}
		else if (type == Variant::DICTIONARY) {
			return to_dictionary(args.get<sol::stack_table>());
		}
	}

	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	internal::gdextension_interface_variant_construct((GDExtensionVariantType) type, &result, (GDExtensionConstVariantPtr *) variant_args.argv(), variant_args.argc(), &error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String msg = "Error constructing " + get_type_name();
		lua_error(args.lua_state(), error, msg);
	}
	return result;
}

bool VariantClass::has_method(const StringName& method) const {
	Variant result;
	GDExtensionCallError error;
	Variant::call_static(type, method, (const Variant **) NULL, 0, result, error);
	return error.error != GDEXTENSION_CALL_ERROR_INVALID_METHOD && error.error != GDEXTENSION_CALL_ERROR_INSTANCE_IS_NULL;
}

static sol::stack_object __index(const VariantClass& cls, const sol::stack_object& key) {
	lua_State *L = key.lua_state();
	if (key.get_type() == sol::type::string) {
		StringName method = key.as<StringName>();
		if (cls.has_method(method)) {
			sol::stack::push(L, MethodBindByName(method));
			return sol::stack_object(L, -1);
		}
	}
	sol::stack::push(L, sol::nil);
	return sol::stack_object(L, -1);
}
void VariantClass::register_usertype(sol::state_view& state) {
	state.new_usertype<VariantClass>(
		"VariantClass",
		sol::meta_function::index, &__index,
		sol::meta_function::call, &VariantClass::construct,
		sol::meta_function::to_string, &VariantClass::get_type_name
	);
}

}
