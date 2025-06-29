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
#include "VariantType.hpp"

#include "Class.hpp"
#include "MethodBindByName.hpp"
#include "VariantArguments.hpp"
#include "VariantTypedArray.hpp"
#include "convert_godot_lua.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

VariantType::VariantType(Variant::Type type) : type(type) {}

Variant::Type VariantType::get_type() const {
	return type;
}

String VariantType::get_type_name() const {
	return Variant::get_type_name(type);
}

Variant VariantType::construct_default() const {
	Variant result;
	GDExtensionCallError error;
	internal::gdextension_interface_variant_construct((GDExtensionVariantType) type, &result, nullptr, 0, &error);
	ERR_FAIL_COND_V_MSG(error.error != GDEXTENSION_CALL_OK, Variant(), "Error constructing " + get_type_name());
	return result;
}

Variant VariantType::construct(const sol::variadic_args& args) const {
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

bool VariantType::operator==(const VariantType& other) const {
	return type == other.type;
}

static sol::object __index(sol::this_state L, const VariantType& cls, const sol::stack_object& key) {
	if (key.get_type() == sol::type::string) {
		StringName method = key.as<StringName>();
		Variant empty = cls.construct_default();
		if (empty.has_method(method)) {
			return sol::make_object(L, MethodBindByName(method));
		}
	}
	else if (cls.get_type() == Variant::Type::ARRAY) {
		if (auto array_type = key.as<sol::optional<VariantType>>()) {
			return sol::make_object(L, VariantTypedArray(array_type->get_type()));
		}
		else if (auto class_name = key.as<sol::optional<Class>>()) {
			return sol::make_object(L, VariantTypedArray(class_name->get_name()));
		}
		else if (Script *script = Object::cast_to<Script>(to_variant(key))) {
			return sol::make_object(L, VariantTypedArray(script));
		}
	}
	return sol::nil;
}
void VariantType::register_usertype(sol::state_view& state) {
	state.new_usertype<VariantType>(
		"VariantClass",
		sol::meta_function::index, &__index,
		sol::meta_function::call, &VariantType::construct,
		sol::meta_function::to_string, &VariantType::get_type_name
	);
}

}
