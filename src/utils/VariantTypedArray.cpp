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
#include "VariantTypedArray.hpp"

#include "convert_godot_lua.hpp"

#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace luagdextension {

VariantTypedArray::VariantTypedArray(Variant::Type type)
	: type(type)
	, class_name()
	, script()
{
}

VariantTypedArray::VariantTypedArray(const StringName& class_name)
	: type(Variant::Type::OBJECT)
	, class_name(class_name)
	, script()
{
}

VariantTypedArray::VariantTypedArray(Script *script)
	: type(Variant::Type::OBJECT)
	, class_name(script->get_instance_base_type())
	, script(script)
{
}

VariantTypedArray VariantTypedArray::of(const Array& array) {
	VariantTypedArray ret;
	ret.type = (Variant::Type) array.get_typed_builtin();
	ret.class_name = array.get_typed_class_name();
	ret.script = array.get_typed_script();
	return ret;
}

Variant::Type VariantTypedArray::get_type() const {
	return type;
}

StringName VariantTypedArray::get_class_name() const {
	if (script.is_valid()) {
		StringName script_name = script->get_global_name();
		if (!script_name.is_empty()) {
			return script_name;
		}
	}
	return class_name;
}

String VariantTypedArray::get_hint_string() const {
	PropertyHint elem_hint = PROPERTY_HINT_NONE;
	String elem_hint_string;

	StringName class_name = get_class_name();
	if (!class_name.is_empty()) {
		if (ClassDBSingleton::get_singleton()->is_parent_class(class_name, Node::get_class_static())) {
			elem_hint = godot::PROPERTY_HINT_NODE_TYPE;
			elem_hint_string = class_name;
		}
		else if (ClassDBSingleton::get_singleton()->is_parent_class(class_name, Resource::get_class_static())) {
			elem_hint = godot::PROPERTY_HINT_RESOURCE_TYPE;
			elem_hint_string = class_name;
		}
	}
	return String("%d/%d:%s") % Array::make(type, elem_hint, elem_hint_string);
}

String VariantTypedArray::to_string() const {
	String element_type;
	if (script.is_valid()) {
		element_type = script->get_global_name();
		if (element_type.is_empty()) {
			element_type = class_name;
		}
	}
	else if (!class_name.is_empty()) {
		element_type = class_name;
	}
	else {
		element_type = Variant::get_type_name(type);
	}
	return String("Array[%s]") % element_type;
}

Variant VariantTypedArray::construct_default() const {
	return Array(Array(), type, class_name, script);
}

Variant VariantTypedArray::construct(const sol::variadic_args& args) const {
	Array array = construct_default();
	if (args.size() == 1 && args.get_type() == sol::type::table) {
		fill_array(array, args.get<sol::stack_table>(0));
	}
	return array;
}

bool VariantTypedArray::operator==(const VariantTypedArray& other) const {
	return other.type == type
		&& other.class_name == class_name
		&& other.script == script;
}

void VariantTypedArray::register_usertype(sol::state_view& state) {
	state.new_usertype<VariantTypedArray>(
		"VariantTypedArray",
		sol::meta_function::call, &VariantTypedArray::construct,
		sol::meta_function::to_string, &VariantTypedArray::to_string
	);
}

};
