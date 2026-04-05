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
#include "VariantType.hpp"

#include "Class.hpp"
#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"
#include "method_bind_impl.hpp"
#include "../generated/variant_type_constants.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

VariantType::VariantType(Variant::Type type)
	: type(type)
	, subtype1()
	, subtype2()
{
}
VariantType::VariantType(Variant::Type type, const Variant& subtype1, const Variant& subtype2)
	: type(type)
	, subtype1(subtype1)
	, subtype2(subtype2)
{
}

VariantType VariantType::from_variant(const Variant& value) {
	switch (value.get_type()) {
		case Variant::Type::ARRAY: {
			Array array = value;
			if (array.is_typed()) {
				return VariantType(Variant::Type::ARRAY, get_subtype(array), Variant());
			}
			break;
		}
		
		case Variant::Type::DICTIONARY: {
			Dictionary dict = value;
			if (dict.is_typed()) {
				return VariantType(Variant::Type::DICTIONARY, get_key_subtype(dict), get_value_subtype(dict));
			}
			break;
		}
		
		default:
			break;
	}
	return VariantType(value.get_type());
}

Variant::Type VariantType::get_type() const {
	return type;
}

bool VariantType::has_type_hints() const {
	return subtype1 || subtype2;
}

PropertyHint VariantType::get_property_hint() const {
	if (type == Variant::Type::ARRAY && has_type_hints()) {
		return PROPERTY_HINT_ARRAY_TYPE;
	}
	else if (type == Variant::Type::DICTIONARY && has_type_hints()) {
		return PROPERTY_HINT_DICTIONARY_TYPE;
	}
	else {
		return PROPERTY_HINT_NONE;
	}
}

String VariantType::get_property_hint_string() const {
	if (type == Variant::Type::ARRAY && has_type_hints()) {
		return subtype_hint_string(subtype1);
	}
	else if (type == Variant::Type::DICTIONARY && has_type_hints()) {
		return subtype_hint_string(subtype1) + ";" + subtype_hint_string(subtype2);
	}
	else {
		return "";
	}
}

String VariantType::to_string() const {
	if (type == Variant::Type::ARRAY && has_type_hints()) {
		return String("Array[%s]") % subtype_name(subtype1);
	}
	else if (type == Variant::Type::DICTIONARY && has_type_hints()) {
		return String("Dictionary[%s, %s]") % Array::make(subtype_name(subtype1), subtype_name(subtype2));
	}
	else {
		return Variant::get_type_name(type);
	}
}

Variant VariantType::construct_default() const {
	if (type == Variant::Type::ARRAY && has_type_hints()) {
		auto [subtype, class_name, script] = subtype_to_constructor_args(subtype1);
		return Array(Array(), subtype, class_name, script);
	}
	else if (type == Variant::Type::DICTIONARY && has_type_hints()) {
		auto [key_subtype, key_class_name, key_script] = subtype_to_constructor_args(subtype1);
		auto [value_subtype, value_class_name, value_script] = subtype_to_constructor_args(subtype2);
		return Dictionary(Dictionary(), key_subtype, key_class_name, key_script, value_subtype, value_class_name, value_script);
	}
	else {
		Variant result;
		GDExtensionCallError error;
		gdextension_interface::variant_construct((GDExtensionVariantType) type, result._native_ptr(), nullptr, 0, &error);
		ERR_FAIL_COND_V_MSG(error.error != GDEXTENSION_CALL_OK, Variant(), "Error constructing " + to_string());
		return result;
	}
}

Variant VariantType::construct(const sol::variadic_args& args) const {
	if (args.size() == 0) {
		return construct_default();
	}
	else if (args.size() == 1 && args.get_type(0) == sol::type::table) {
		if (type == Variant::ARRAY) {
			Array array = construct_default();
			fill_array(array, args.get<sol::stack_table>());
			return array;
		}
		else if (type == Variant::DICTIONARY) {
			Dictionary dictionary = construct_default();
			fill_dictionary(dictionary, args.get<sol::stack_table>());
			return dictionary;
		}
	}

	VariantArguments variant_args = args;
	Variant result;
	GDExtensionCallError error;
	gdextension_interface::variant_construct((GDExtensionVariantType) type, result._native_ptr(), (GDExtensionConstVariantPtr *) variant_args.argv(), variant_args.argc(), &error);
	if (error.error != GDEXTENSION_CALL_OK) {
		String msg = "Error constructing " + to_string();
		lua_error(args.lua_state(), error, msg);
	}
	return result;
}

bool VariantType::operator==(const VariantType& other) const {
	return type == other.type
		&& subtype1 == other.subtype1
		&& subtype2 == other.subtype2;
}

sol::object VariantType::__index(sol::this_state L, const VariantType& type, const sol::stack_object& key) {
	if (key.get_type() == sol::type::string) {
		StringName key_str = key.as<StringName>();

		Variant constant = variant_constant_named(type.get_type(), key_str);
		if (constant.get_type() != Variant::NIL) {
			return to_lua(L, constant);
		}

		Variant empty = type.construct_default();
		if (empty.has_method(key_str)) {
			return sol::make_object(L, VariantTypeMethodBind(type, key_str));
		}
		else {
			return sol::nil;
		}
	}
	
	Variant new_subtype;
	if (auto subtype = key.as<sol::optional<VariantType>>()) {
		new_subtype = subtype->get_type();
	}
	else if (auto cls = key.as<sol::optional<Class>>()) {
		new_subtype = cls->get_name();
	}
	else if (Script *script = Object::cast_to<Script>(to_variant(key))) {
		new_subtype = script;
	}
	else if (sol::state_view(L).globals().get<sol::object>("Variant") == key) {
		new_subtype = false;
	}

	if (new_subtype != Variant() && (type.get_type() == Variant::Type::ARRAY || type.get_type() == Variant::Type::DICTIONARY)) {
		if (type.subtype1 == Variant()) {
			return sol::make_object(L, VariantType(type.get_type(), new_subtype, Variant()));
		}
		else if (type.subtype2 == Variant()) {
			return sol::make_object(L, VariantType(type.get_type(), type.subtype1, new_subtype));
		}
	}
	return sol::nil;
}
void VariantType::register_usertype(sol::state_view& state) {
	state.new_usertype<VariantType>(
		"VariantClass",
		sol::meta_function::index, &VariantType::__index,
		sol::meta_function::call, &VariantType::construct,
		sol::meta_function::to_string, &VariantType::to_string
	);
	VariantTypeMethodBind::register_usertype(state);
}

std::tuple<Variant::Type, StringName, Variant> VariantType::subtype_to_constructor_args(const Variant& subtype) {
	switch (subtype.get_type()) {
		case Variant::Type::NIL:
		case Variant::Type::BOOL:
			return std::make_tuple(Variant::Type::NIL, StringName(), Variant());

		case Variant::Type::INT:
			return std::make_tuple((Variant::Type)(int) subtype, StringName(), Variant());
			
		case Variant::Type::STRING:
		case Variant::Type::STRING_NAME:
			return std::make_tuple(Variant::Type::OBJECT, subtype, Variant());
			
		case Variant::Type::OBJECT: {
			StringName class_name;
			if (Script *script = Object::cast_to<Script>(subtype)) {
				class_name = script->get_instance_base_type();
			}
			return std::make_tuple(Variant::Type::OBJECT, class_name, subtype);
		}

		default:
			ERR_FAIL_V_MSG({}, String("FIXME: Subtype type not supported: ") + Variant::get_type_name(subtype.get_type()));
	}
}

String VariantType::subtype_name(const Variant& subtype) {
	switch (subtype.get_type()) {
		case Variant::Type::NIL:
		case Variant::Type::BOOL:
			return "Variant";

		case Variant::Type::INT:
			return Variant::get_type_name((Variant::Type)(int) subtype);
			
		case Variant::Type::STRING:
		case Variant::Type::STRING_NAME:
			return subtype;
			
		case Variant::Type::OBJECT: {
			StringName class_name;
			if (Script *script = Object::cast_to<Script>(subtype)) {
				class_name = script->get_global_name();
				if (class_name.is_empty()) {
					class_name = script->get_instance_base_type();
				}
			}
			return class_name;
		}

		default:
			ERR_FAIL_V_MSG({}, String("FIXME: Subtype type not supported: ") + Variant::get_type_name(subtype.get_type()));
	}
}

String VariantType::subtype_hint_string(const Variant& subtype) {
	Variant::Type hint_type;
	StringName hint_class_name;
	PropertyHint property_hint = PROPERTY_HINT_NONE;

	switch (subtype.get_type()) {
		case Variant::Type::NIL:
		case Variant::Type::BOOL:
			return "";

		case Variant::Type::INT:
			hint_type = (Variant::Type)(int) subtype;
			break;
			
		case Variant::Type::STRING:
		case Variant::Type::STRING_NAME:
			hint_type = Variant::Type::OBJECT;
			hint_class_name = subtype;
			if (ClassDB::is_parent_class(hint_class_name, Node::get_class_static())) {
				property_hint = PROPERTY_HINT_NODE_TYPE;
			}
			else if (ClassDB::is_parent_class(hint_class_name, Resource::get_class_static())) {
				property_hint = PROPERTY_HINT_RESOURCE_TYPE;
			}
			break;
			
		case Variant::Type::OBJECT:
			if (Script *script = Object::cast_to<Script>(subtype)) {
				hint_type = Variant::Type::OBJECT;
				StringName class_name = script->get_instance_base_type();
				hint_class_name = script->get_global_name();
				if (hint_class_name.is_empty()) {
					hint_class_name = class_name;
				}
				if (ClassDB::is_parent_class(class_name, Node::get_class_static())) {
					property_hint = PROPERTY_HINT_NODE_TYPE;
				}
				else if (ClassDB::is_parent_class(class_name, Resource::get_class_static())) {
					property_hint = PROPERTY_HINT_RESOURCE_TYPE;
				}
			}
			break;

		default:
			ERR_FAIL_V_MSG({}, String("FIXME: Subtype type not supported: ") + Variant::get_type_name(subtype.get_type()));
	}

	return String("%d/%d:%s") % Array::make(hint_type, property_hint, hint_class_name);
}

Variant VariantType::get_subtype(const Array& array) {
	if (Variant script = array.get_typed_script()) {
		return script;
	}
	else if (StringName class_name = array.get_typed_class_name(); !class_name.is_empty()) {
		return class_name;
	}
	else {
		return array.get_typed_builtin();
	}
}

Variant VariantType::get_key_subtype(const Dictionary& dict) {
	if (Variant script = dict.get_typed_key_script()) {
		return script;
	}
	else if (StringName class_name = dict.get_typed_key_class_name(); !class_name.is_empty()) {
		return class_name;
	}
	else {
		return dict.get_typed_key_builtin();
	}
}

Variant VariantType::get_value_subtype(const Dictionary& dict) {
	if (Variant script = dict.get_typed_value_script()) {
		return script;
	}
	else if (StringName class_name = dict.get_typed_value_class_name(); !class_name.is_empty()) {
		return class_name;
	}
	else {
		return dict.get_typed_value_builtin();
	}
}

}
