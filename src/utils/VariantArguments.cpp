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
#include "VariantArguments.hpp"

#include "convert_godot_lua.hpp"

namespace luagdextension {

VariantArguments::VariantArguments(const Array& args)
	: variants(args)
{
}

VariantArguments::VariantArguments(const Variant **argv, GDExtensionInt argc) {
	for (GDExtensionInt i = 0; i < argc; i++) {
		variants.append(*argv[i]);
	}
}

VariantArguments::VariantArguments(const Variant& self, const Variant **argv, GDExtensionInt argc) {
	variants.append(self);
	for (GDExtensionInt i = 0; i < argc; i++) {
		variants.append(*argv[i]);
	}
}

VariantArguments::VariantArguments(const sol::variadic_args& args) {
	fill_array(variants, args);
}

int VariantArguments::argc() const {
	return variants.size();
}

const Variant **VariantArguments::argv() {
	for (int64_t i = variant_pointers.size(); i < variants.size(); i++) {
		variant_pointers.append(&variants[i]);
	}
	return variant_pointers.ptrw();
}

const Array& VariantArguments::get_array() const {
	return variants;
}

}
