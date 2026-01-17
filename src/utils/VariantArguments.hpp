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
#ifndef __UTILS_VARIANT_ARGUMENTS_HPP__
#define __UTILS_VARIANT_ARGUMENTS_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

/**
 * Convert between sol::variadic_args to Variant argc/argv.
 */
class VariantArguments {
	Array variants;
	Vector<const Variant *> variant_pointers;

public:
	VariantArguments() = default;
	VariantArguments(const Array& args);
	VariantArguments(const Variant **argv, GDExtensionInt argc);
	VariantArguments(const Variant& self, const Variant **argv, GDExtensionInt argc);
	VariantArguments(const sol::variadic_args& args);

	int argc() const;
	const Variant **argv();
	const Array& get_array() const;
};

}

#endif  // __UTILS_VARIANT_ARGUMENTS_HPP__
