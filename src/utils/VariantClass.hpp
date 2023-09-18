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
#ifndef __UTILS_VARIANT_CLASS_HPP__
#define __UTILS_VARIANT_CLASS_HPP__

#include "custom_sol.hpp"
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

/**
 * Object that represents Godot's builtin classes (a.k.a. Variants) in Lua.
 */
class VariantClass {
protected:
	Variant::Type type;

public:
	VariantClass(Variant::Type type);

	Variant::Type get_type() const;
	String get_type_name() const;

	Variant construct(const sol::variadic_args& args);
};

}

#endif  // __UTILS_VARIANT_CLASS_HPP__
