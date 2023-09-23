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
#include "convert_godot_lua.hpp"
#include "convert_godot_std.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

MethodBindByName::MethodBindByName(const StringName& method_name) : method_name(method_name) {}

StringName MethodBindByName::get_method_name() const {
	return method_name;
}

sol::stack_object MethodBindByName::call(Variant& variant, const sol::variadic_args& args, sol::this_state state) const {
	return variant_call(variant, method_name, args, state);
}

std::tuple<bool, sol::stack_object> MethodBindByName::pcall(Variant& variant, const sol::variadic_args& args, sol::this_state state) const {
	return variant_pcall(variant, method_name, args, state);
}

void MethodBindByName::register_usertype(sol::state_view& state) {
	state.new_usertype<MethodBindByName>(
		"MethodBindByName",
		sol::call_constructor, sol::constructors<
			MethodBindByName(const StringName&)
		>(),
		"call", &MethodBindByName::call,
		"pcall", &MethodBindByName::pcall,
		sol::meta_function::call, &MethodBindByName::call,
		sol::meta_function::to_string, &MethodBindByName::get_method_name
	);
}

}
