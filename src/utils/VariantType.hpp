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
#ifndef __UTILS_VARIANT_TYPE_HPP__
#define __UTILS_VARIANT_TYPE_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

/**
 * Object that represents Godot's builtin classes (a.k.a. Variants) in Lua.
 */
class VariantType {
public:
	VariantType(Variant::Type type);
	static VariantType from_variant(const Variant& value);

	Variant::Type get_type() const;
	bool has_type_hints() const;
	PropertyHint get_property_hint() const;
	String get_property_hint_string() const;
	String to_string() const;

	Variant construct_default() const;
	Variant construct(const sol::variadic_args& args) const;

	bool operator==(const VariantType& other) const;

	static void register_usertype(sol::state_view& state);

protected:
	Variant::Type type;
	
	// Used for typed arrays and typed dictionaries
	Variant subtype1;
	// Used for typed dictionaries
	Variant subtype2;

	VariantType(Variant::Type type, const Variant& subtype1, const Variant& subtype2);

	static sol::object __index(sol::this_state L, const VariantType& cls, const sol::stack_object& key);
	
	static std::tuple<Variant::Type, StringName, Variant> subtype_to_constructor_args(const Variant& subtype);
	static String subtype_name(const Variant& subtype);
	static String subtype_hint_string(const Variant& subtype);
	static Variant get_subtype(const Array& array);
	static Variant get_key_subtype(const Dictionary& dict);
	static Variant get_value_subtype(const Dictionary& dict);
};

}

#endif  // __UTILS_VARIANT_TYPE_HPP__
