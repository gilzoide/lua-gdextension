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
#include "VariantClass.hpp"

#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

VariantClass::VariantClass(Variant::Type type) : type(type) {}

Variant::Type VariantClass::get_type() const {
	return type;
}

String VariantClass::get_type_name() const {
	return Variant::get_type_name(type);
}

Variant VariantClass::construct(const sol::variadic_args& args) {
	if (args.size() == 1 && args.get_type() == sol::type::table) {
		if (type == Variant::ARRAY) {
			return to_array(args.get<sol::stack_table>());
		}
		else if (type == Variant::DICTIONARY) {
			return to_dictionary(args.get<sol::stack_table>());
		}
	}

	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	internal::gdextension_interface_variant_construct((GDExtensionVariantType) type, &result, (GDExtensionConstVariantPtr *) variant_args.argv(), variant_args.argc(), &error);
	if (error.error != GDEXTENSION_CALL_OK) {
		luaL_error(args.lua_state(), "Error in constructor. TODO: format error information");
	}
	return result;
}

}
