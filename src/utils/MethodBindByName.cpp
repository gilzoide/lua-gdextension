/**
 * Copyright (C) 2023 Gil Barbosa Reis.
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

#include "VariantArguments.hpp"
#include "VariantClass.hpp"
#include "convert_godot_lua.hpp"
#include "convert_godot_std.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

MethodBindByName::MethodBindByName(const StringName& method_name) : method_name(method_name) {}

const StringName& MethodBindByName::get_method_name() const {
	return method_name;
}

sol::stack_object MethodBindByName::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	if (self.is<VariantClass>()) {
		VariantClass var_type = self.as<VariantClass>();
		return variant_static_call_string_name(state, var_type.get_type(), method_name, args);
	}
	else {
		Variant variant = to_variant(self);
		return variant_call_string_name(state, variant, method_name, args);
	}
}

void MethodBindByName::register_usertype(sol::state_view& state) {
	state.new_usertype<MethodBindByName>(
		"MethodBindByName",
		sol::call_constructor, sol::constructors<
			MethodBindByName(const StringName&)
		>(),
		"call", &MethodBindByName::call,
		sol::meta_function::call, &MethodBindByName::call,
		sol::meta_function::to_string, &MethodBindByName::get_method_name
	);
}

}
