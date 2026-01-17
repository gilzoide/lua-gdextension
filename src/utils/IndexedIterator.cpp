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
#include "IndexedIterator.hpp"

#include "convert_godot_lua.hpp"

namespace luagdextension {

IndexedIterator::IndexedIterator(const Variant& variant)
	: variant(variant)
	, index(-1)
{
}

std::tuple<sol::object, sol::object> IndexedIterator::iter_next_lua(sol::this_state state) {
	index++;
	bool is_valid, is_out_of_bounds;
	Variant result = variant.get_indexed(index, is_valid, is_out_of_bounds);
	if (is_valid && !is_out_of_bounds) {
		return std::make_tuple(sol::make_object(state, index), to_lua(state, result));
	}
	else {
		return std::make_tuple(sol::nil, sol::nil);
	}
}

bool IndexedIterator::supports_indexed_pairs(const Variant& variant) {
	switch (variant.get_type()) {
		case Variant::ARRAY:
		case Variant::PACKED_BYTE_ARRAY:
		case Variant::PACKED_INT32_ARRAY:
		case Variant::PACKED_INT64_ARRAY:
		case Variant::PACKED_FLOAT32_ARRAY:
		case Variant::PACKED_FLOAT64_ARRAY:
		case Variant::PACKED_STRING_ARRAY:
		case Variant::PACKED_VECTOR2_ARRAY:
		case Variant::PACKED_VECTOR3_ARRAY:
		case Variant::PACKED_COLOR_ARRAY:
		case Variant::PACKED_VECTOR4_ARRAY:
			return true;

		default:
			break;
	}

	bool is_valid, is_out_of_bounds;
	variant.get_indexed(0, is_valid, is_out_of_bounds);
	return is_valid;
}

std::tuple<sol::object, sol::object> IndexedIterator::indexed_pairs(sol::this_state state, const Variant& indexed) {
	return std::make_tuple(
		sol::make_object(state, &IndexedIterator::iter_next_lua),
		sol::make_object(state, IndexedIterator(indexed))
	);
}

}
