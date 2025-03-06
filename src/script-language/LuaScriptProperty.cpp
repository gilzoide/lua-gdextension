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

#include "../utils/VariantType.hpp"
#include "../utils/convert_godot_lua.hpp"
#include "godot_cpp/core/property_info.hpp"

namespace luagdextension {

Variant LuaScriptProperty::instantiate_value() const {
	if (default_value.get_type() == type) {
		return default_value.duplicate();
	}
	else {
		return VariantType(type).construct_default();
	}
}

PropertyInfo LuaScriptProperty::to_property_info() const {
	return PropertyInfo(type, name);
}

Dictionary LuaScriptProperty::to_dictionary() const {
	return to_property_info();
}

LuaScriptProperty LuaScriptProperty::from_lua(sol::stack_object value) {
	LuaScriptProperty property;
	if (auto table = value.as<sol::optional<sol::stack_table>>()) {
		if (auto default_value = table->get<sol::optional<sol::object>>("default")) {
			property.default_value = to_variant(*default_value);
		}
		else if (auto default_value = table->get<sol::optional<sol::object>>(1)) {
			property.default_value = to_variant(*default_value);
		}
		
		if (auto type = table->get<sol::optional<VariantType>>("type")) {
			property.type = type->get_type();
		}
		else if (auto type = table->get<sol::optional<VariantType>>(2)) {
			property.type = type->get_type();
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
	return property;
}

}
