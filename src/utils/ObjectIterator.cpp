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
#include "ObjectIterator.hpp"

#include "convert_godot_lua.hpp"
#include "convert_godot_std.hpp"

namespace luagdextension {

ObjectIterator::ObjectIterator(const Variant& variant, const Variant& iterator)
		: variant(variant), iterator(iterator) {}

Variant ObjectIterator::iter_next() {
	bool is_valid;
	if (variant.iter_next(iterator, is_valid)) {
		return variant.iter_get(iterator, is_valid);
	}
	else {
		return Variant();
	}
}

sol::object ObjectIterator::iter_next_lua(sol::this_state state) {
	bool is_valid;
	if (variant.iter_next(iterator, is_valid)) {
		return to_lua(state, variant.iter_get(iterator, is_valid));
	}
	else {
		return sol::nil;
	}
}

std::tuple<sol::object, sol::object> ObjectIterator::object_pairs(sol::this_state state, const Variant& variant) {
	bool is_valid;
	Variant iterator = variant.iter_get(variant, is_valid);
	if (is_valid) {
		return std::make_tuple(
			sol::make_object(state, &ObjectIterator::iter_next_lua),
			sol::make_object(state, ObjectIterator(variant, iterator))
		);
	}
	else {
		CharString var_type = get_type_name(variant).ascii();
		luaL_error(state, "Object of type %s does not support 'pairs' iteration", var_type.get_data());
		return {};
	}
}

}
