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

#include "LuaScriptProperty.hpp"
#include "LuaScriptInstance.hpp"

#include "../LuaFunction.hpp"
#include "../utils/VariantType.hpp"
#include "../utils/convert_godot_lua.hpp"

namespace luagdextension {

static LuaScriptProperty lua_property(sol::stack_object value) {
	LuaScriptProperty property;
	property.usage = PROPERTY_USAGE_STORAGE;

	if (auto table = value.as<sol::optional<sol::stack_table>>()) {
		// index 1: either a Variant type or the default value
		if (auto type = table->get<sol::optional<VariantType>>(1)) {
			property.type = type->get_type();
		}
		else if (auto default_value = table->get<sol::optional<sol::object>>(1)) {
			property.default_value = to_variant(*default_value);
		}

		// PropertyInfo fields
		if (auto type = table->get<sol::optional<VariantType>>("type")) {
			property.type = type->get_type();
		}
		if (auto hint = table->get<sol::optional<uint32_t>>("hint")) {
			property.hint = *hint;
		}
		if (auto hint_string = table->get<sol::optional<String>>("hint_string")) {
			property.hint_string = *hint_string;
		}
		if (auto usage = table->get<sol::optional<uint32_t>>("usage")) {
			property.usage = *usage;
		}
		if (auto class_name = table->get<sol::optional<StringName>>("class_name")) {
			property.class_name = *class_name;
		}

		// Extra LuaScriptProperty fields
		if (auto default_value = table->get<sol::optional<sol::object>>("default")) {
			property.default_value = to_variant(*default_value);
		}
		if (auto getter_name = table->get<sol::optional<StringName>>("get")) {
			property.getter_name = *getter_name;
			property.usage &= ~PROPERTY_USAGE_STORAGE;
		}
		else if (auto getter = table->get<sol::optional<sol::protected_function>>("get")) {
			property.getter = getter;
			property.usage &= ~PROPERTY_USAGE_STORAGE;
		}
		if (auto setter_name = table->get<sol::optional<StringName>>("set")) {
			property.setter_name = *setter_name;
		}
		else if (auto setter = table->get<sol::optional<sol::protected_function>>("set")) {
			property.setter = setter;
		}
	}
	else if (auto type = value.as<sol::optional<VariantType>>()) {
		property.type = type->get_type();
	}
	else {
		property.default_value = to_variant(value);
	}
	
	if (property.type == 0) {
		property.type = property.default_value.get_type();
	}
	property.usage |= PROPERTY_USAGE_SCRIPT_VARIABLE;
	return property;
}

static LuaScriptProperty lua_export(sol::stack_object value) {
	LuaScriptProperty property = lua_property(value);
	property.usage |= PROPERTY_USAGE_DEFAULT;
	return property;
}

LuaScriptProperty::LuaScriptProperty(const Variant& value, const StringName& name)
	: type(value.get_type())
	, name(name)
	, default_value(value)
{
}

bool LuaScriptProperty::get_value(LuaScriptInstance *self, Variant& r_value) const {
	if (getter) {
		r_value = LuaFunction::invoke_method_lua(*getter, self->owner, nullptr, 0, false);
		return true;
	}
	if (!getter_name.is_empty()) {
		r_value = self->owner->call(getter_name);
		return true;
	}
	else {
		return false;
	}
}

Variant LuaScriptProperty::instantiate_default_value() const {
	if (default_value.get_type() == type) {
		return default_value.duplicate();
	}
	else {
		return VariantType(type).construct_default();
	}
}

bool LuaScriptProperty::set_value(LuaScriptInstance *self, const Variant& value) const {
	if (setter) {
		LuaFunction::invokev_lua(*setter, Array::make(self->owner, value), false);
		return true;
	}
	else if (!setter_name.is_empty()) {
		self->owner->call(setter_name, value);
		return true;
	}
	else {
		return false;
	}
}

PropertyInfo LuaScriptProperty::to_property_info() const {
	return PropertyInfo(type, name, (PropertyHint) hint, hint_string, (PropertyUsageFlags) usage, class_name);
}

Dictionary LuaScriptProperty::to_dictionary() const {
	return to_property_info();
}

void LuaScriptProperty::register_lua(lua_State *L) {
	sol::state_view state(L);

	state.new_usertype<LuaScriptProperty>(
		"LuaScriptProperty",
		sol::no_construction()
	);
	state.set("property", &lua_property);
	state.set("export", &lua_export);
}

}
