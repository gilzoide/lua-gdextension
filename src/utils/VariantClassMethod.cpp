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
#include "VariantClassMethod.hpp"

#include "VariantArguments.hpp"

namespace luagdextension {

VariantClassMethod::VariantClassMethod(Variant::Type type, const StringName& method_name) : type(type), method_name(method_name) {}

Variant::Type VariantClassMethod::get_type() const {
	return type;
}

const StringName& VariantClassMethod::get_method_name() const {
	return method_name;
}

Variant VariantClassMethod::call(const sol::variadic_args& args) const {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	Variant::call_static(type, method_name, variant_args.argv(), variant_args.argc(), result, error);
	if (error.error != GDEXTENSION_CALL_OK) {
		PackedByteArray method_name_utf8 = method_name.to_utf8_buffer();
		luaL_error(args.lua_state(), "Invalid static call to method %s in Variant of type %d", method_name_utf8.ptr(), type);
	}
	return result;
}

void VariantClassMethod::register_usertype(sol::state_view& state) {
	state.new_usertype<VariantClassMethod>(
		"VariantClassMethod",
		sol::call_constructor, sol::constructors<
			VariantClassMethod(Variant::Type, const StringName&)
		>(),
		sol::meta_function::call, &VariantClassMethod::call
	);
}

}
