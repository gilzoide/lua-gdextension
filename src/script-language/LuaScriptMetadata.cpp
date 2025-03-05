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

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "LuaScriptMetadata.hpp"
#include "../utils/convert_godot_lua.hpp"
#include "../utils/stack_top_checker.hpp"

namespace luagdextension {

void LuaScriptMetadata::setup(const sol::table& t) {
	is_valid = true;

	sol::state_view L = t.lua_state();
	StackTopChecker topcheck(L);
	auto tablepop = sol::stack::push_pop(L, t);
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		sol::stack_object key(L, -2);
		sol::stack_object value(L, -1);
		if (key.get_type() != sol::type::string) {
			lua_pop(L, 1);
			continue;
		}

		String name = key.as<String>();
		if (name == "extends") {
			StringName extends = to_variant(value);
			if (!ClassDB::class_exists(extends)) {
				WARN_PRINT(String("Specified base class '%s' does not exist, using RefCounted") % Array::make(extends));
			}
			else {
				base_class = extends;
			}
		}
		else if (name == "class_name") {
			class_name = to_variant(value);
		}
		else if (name == "icon") {
			icon_path = to_variant(value);
		}
		else if (name == "tool") {
			is_tool = true;
		}
		else if (auto signal = value.as<sol::optional<LuaScriptSignal>>()) {
			signals.insert(name, *signal);
		}
		else if (auto property = value.as<sol::optional<LuaScriptProperty>>()) {
			properties.insert(name, *property);
		}
		else if (auto method = value.as<sol::optional<sol::protected_function>>()) {
			methods.insert(name, *method);
		}
		else {
			Variant var = to_variant(value);
			properties.insert(name, LuaScriptProperty {
				.type = var.get_type(),
				.default_value = var,
				.getter = sol::nullopt,
				.setter = sol::nullopt,
			});
		}

		lua_pop(L, 1);
	}
}

void LuaScriptMetadata::clear() {
	is_valid = false;
	is_tool = false;
	base_class = RefCounted::get_class_static();
	class_name = StringName();
	icon_path = String();
	properties.clear();
	signals.clear();
	methods.clear();
}

}
