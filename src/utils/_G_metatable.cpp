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
#include "_G_metatable.hpp"

#include "Class.hpp"
#include "convert_godot_lua.hpp"
#include "module_names.hpp"
#include "../script-language/LuaScriptLanguage.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace luagdextension {

sol::object __index(sol::this_state state, sol::global_table _G, sol::stack_object key) {
	static Engine *engine = Engine::get_singleton();

	if (key.get_type() != sol::type::string) {
		return sol::nil;
	}

	auto registry = sol::state_view(state).registry();
	if (registry.get_or(module_names::singleton_access, false)) {
		auto class_name = key.as<StringName>();
		if (engine->has_singleton(class_name)) {
			Variant singleton = engine->get_singleton(class_name);
			return _G[key] = to_lua(state, singleton);
		}
		else if (Variant named_global = LuaScriptLanguage::get_singleton()->get_named_globals().get(class_name, nullptr); named_global.get_type() != Variant::NIL) {
			return _G[key] = to_lua(state, named_global);
		}
	}
	if (registry.get_or(module_names::classes, false)) {
		StringName class_name = key.as<StringName>();
		if (ClassDB::class_exists(class_name)) {
			Class cls(class_name);
			return _G[key] = sol::make_object(state, cls);
		}
	}
	return sol::nil;
}

void setup_G_metatable(sol::state_view& state) {
	state.globals()[sol::metatable_key] = state.create_table_with(
		sol::meta_function::index, &__index
	);
}

}

