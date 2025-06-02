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
#ifndef __LUA_SCRIPT_PROPERTY_HPP__
#define __LUA_SCRIPT_PROPERTY_HPP__

#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "../LuaFunction.hpp"

using namespace godot;

namespace luagdextension {

class LuaScriptInstance;

struct LuaScriptProperty {
	Variant::Type type = Variant::NIL;
	StringName name;
	StringName class_name;
	uint32_t hint = PROPERTY_HINT_NONE;
	String hint_string;
	uint32_t usage = PROPERTY_USAGE_SCRIPT_VARIABLE;

	LuaScriptProperty() = default;
	LuaScriptProperty(const Variant& value, const StringName& name);

	Variant default_value;

	StringName getter_name;
	StringName setter_name;
	Ref<LuaFunction> getter;  // Variant getter(self)
	Ref<LuaFunction> setter;  // void setter(self, Variant value)

	bool get_value(LuaScriptInstance *self, Variant& r_value) const;
	bool set_value(LuaScriptInstance *self, const Variant& value) const;
	Variant instantiate_default_value() const;

	PropertyInfo to_property_info() const;
	Dictionary to_dictionary() const;

	static void register_lua(lua_State *L);
};

}

#endif  // __LUA_SCRIPT_PROPERTY_HPP__
