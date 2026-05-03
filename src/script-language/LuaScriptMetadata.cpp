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
#include "LuaScriptMetadata.hpp"

#include "LuaScript.hpp"
#include "LuaScriptInstance.hpp"
#include "../utils/convert_godot_lua.hpp"
#include "../utils/extra_utility_functions.hpp"
#include "../utils/stack_top_resetter.hpp"
#include "../utils/string_names.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/script_language.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string_name.hpp>

namespace luagdextension {

static sol::stateless_reference _G_pairs;

void LuaScriptMetadata::setup(const sol::table& t, const LuaScript* script) {
	is_valid = true;

	sol::state_view L = t.lua_state();
	StackTopResetter topreset(L);

	// Global methods
	methods[string_names->rawget] = LuaScriptMethod(string_names->rawget, LuaScriptInstance::rawget);
	methods[string_names->rawset] = LuaScriptMethod(string_names->rawset, LuaScriptInstance::rawset);

	_G_pairs.push(L);
	t.push();
	if (lua_pcall(L, 1, 3, 0) != LUA_OK) {
		ERR_FAIL_MSG(luaL_tolstring(L, -1, nullptr));
	}

	while (true) {
		lua_pushvalue(L, -3);
		lua_insert(L, -3);
		if (lua_pcall(L, 2, 2, 0) != LUA_OK) {
			ERR_FAIL_MSG(luaL_tolstring(L, -1, nullptr));
		}

		sol::stack_object key(L, -2);
		if (key.get_type() == sol::type::nil) {
			break;
		} else if (key.get_type() == sol::type::string) {
			sol::stack_object value(L, -1);

			String name = key.as<String>();
			if (name == "extends") {
				StringName extends = to_variant(value);
				if (!ClassDB::class_exists(extends)) {
					// Try find in global class list
					Ref<Script> extends_script = get_class_script(extends);
					// Try find by abosolute path
					if (extends_script.is_null() and ResourceLoader::get_singleton()->exists(extends))
						extends_script = ResourceLoader::get_singleton()->load(extends, "Script");
					// Try find by relative path
					if (extends_script.is_null()) {
						if (String full_path = script->get_path().get_base_dir().path_join(extends); ResourceLoader::get_singleton()->exists(full_path))
							extends_script = ResourceLoader::get_singleton()->load(full_path, "Script");
					}

					if (extends_script.is_null()) {
						WARN_PRINT(String("Specified base class '%s' does not exist, using RefCounted") % Array::make(extends));
					} else {
						base_class = static_cast<Ref<Script>>(extends_script)->get_instance_base_type();
						base_script = extends_script;
					}
				} else {
					base_class = extends;
					base_script = Ref<Script>{nullptr};
				}
			} else if (name == "class_name") {
				class_name = to_variant(value);
			} else if (name == "icon") {
				icon_path = to_variant(value);
			} else if (name == "tool") {
				is_tool = to_variant(value).booleanize();
			} else if (name == "abstract") {
				is_abstract = to_variant(value).booleanize();
			} else if (name == "rpc_config") {
				if (value.get_type() == sol::type::table) {
					rpc_config = to_dictionary(value.as<sol::stack_table>());
				} else {
					rpc_config = to_variant(value);
				}
			} else if (auto signal = value.as<sol::optional<LuaScriptSignal>>()) {
				signal->name = name;
				signals.insert(name, *signal);
			} else if (auto property = value.as<sol::optional<LuaScriptProperty>>()) {
				property->name = name;
				properties.insert(name, *property);
			} else if (value.get_type() == sol::type::function) {
				methods.insert(name, LuaScriptMethod(name, value));
			} else {
				Variant var = to_variant(value);
				properties.insert(name, LuaScriptProperty(var, name));
			}
		}

		// pop value
		lua_pop(L, 1);
		// insert table between function and key
		t.push(L);
		lua_insert(L, -2);
	}
}

void LuaScriptMetadata::clear() {
	is_valid = false;
	is_tool = false;
	is_abstract = false;
	base_class = RefCounted::get_class_static();
	base_script = Ref<Script>{nullptr};
	class_name = StringName();
	icon_path = String();
	rpc_config = Variant();
	properties.clear();
	signals.clear();
	methods.clear();
}

void LuaScriptMetadata::register_lua(lua_State* L) {
	_G_pairs = sol::state_view(L).globals()["pairs"];
}

}
