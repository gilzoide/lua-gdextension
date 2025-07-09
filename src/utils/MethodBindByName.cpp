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
#include "MethodBindByName.hpp"

#include "Class.hpp"
#include "VariantArguments.hpp"
#include "VariantType.hpp"
#include "convert_godot_lua.hpp"
#include "string_names.hpp"

#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

MethodBindByName::MethodBindByName(const StringName& method_name) : method_name(method_name) {}

const StringName& MethodBindByName::get_method_name() const {
	return method_name;
}

sol::object MethodBindByName::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	if (self.get_type() == sol::type::none) {
		luaL_error(state, "To call methods, use ':' instead of '.': `obj:%s(...)`", String(method_name).ascii().get_data());
	}

	if (auto var_type = self.as<sol::optional<VariantType>>()) {
		return variant_static_call_string_name(state, var_type->get_type(), method_name, args);
	}
	else if (auto cls = self.as<sol::optional<Class>>()) {
		Array var_args = VariantArguments(args).get_array();
		var_args.push_front(method_name);
		var_args.push_front(cls->get_name());
		return to_lua(state, ClassDBSingleton::get_singleton()->callv(string_names->class_call_static, var_args));
	}
	else {
		Variant variant = to_variant(self);
		return variant_call_string_name(state, variant, method_name, args);
	}
}

void MethodBindByName::register_usertype(sol::state_view& state) {
	state.new_usertype<MethodBindByName>(
		"MethodBindByName",
		sol::no_constructor,
		sol::meta_function::call, &MethodBindByName::call,
		sol::meta_function::to_string, &MethodBindByName::get_method_name
	);
}

}
