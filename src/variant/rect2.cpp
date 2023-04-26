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
#include "register_types.hpp"

#include "../constants.hpp"

#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/rect2i.hpp>
#include <godot_cpp/variant/transform2d.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

void register_rect2(sol::state_view& state) {
	state.new_usertype<Rect2>(
		"Rect2",
		// constructors
		sol::call_constructor, sol::constructors<Rect2(), Rect2(real_t, real_t, real_t, real_t), Rect2(const Point2&, const Size2&)>(),
		// properties
		"position", &Rect2::position,
		"size", &Rect2::size,
		"end", sol::property(&Rect2::set_end, &Rect2::get_end),
		// constants
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::RECT2),
		// methods
		"get_area", &Rect2::get_area,
		"get_center", &Rect2::get_center,
		"intersects", &Rect2::intersects,
		"distance_to", &Rect2::distance_to,
		"intersects_transformed", &Rect2::intersects_transformed,
		"intersects_segment", &Rect2::intersects_segment,
		"encloses", &Rect2::encloses,
		"has_area", &Rect2::has_area,
		"intersection", &Rect2::intersection,
		"merge", &Rect2::merge,
		"has_point", &Rect2::has_point,
		"is_equal_approx", &Rect2::is_equal_approx,
		"grow", &Rect2::grow,
		"grow_by", &Rect2::grow_by,
		"grow_side", &Rect2::grow_side,
		"grow_individual", &Rect2::grow_individual,
		"expand", &Rect2::expand,
		"expand_to", &Rect2::expand_to,
		"abs", &Rect2::abs,
		"get_support", &Rect2::get_support,
		// cast methods
		"to_string", &Rect2::operator String,
		"to_rect2i", &Rect2::operator Rect2i,
		// operators
		sol::meta_function::to_string, &Rect2::operator String
	);
}

}
