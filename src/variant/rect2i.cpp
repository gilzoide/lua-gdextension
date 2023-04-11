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

#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/rect2i.hpp>
#include <godot_cpp/variant/transform2d.hpp>

using namespace godot;

namespace luagdextension {

void register_rect2i(sol::state_view& state) {
	state.new_usertype<Rect2i>(
		"Rect2i",
		// constructors
		sol::call_constructor, sol::constructors<Rect2i(), Rect2i(int, int, int, int), Rect2i(const Point2i&, const Size2i&)>(),
		// properties
		"position", &Rect2i::position,
		"size", &Rect2i::size,
		"end", sol::property(&Rect2i::set_end, &Rect2i::get_end),
		// methods
		"get_area", &Rect2i::get_area,
		"get_center", &Rect2i::get_center,
		"intersects", &Rect2i::intersects,
		"encloses", &Rect2i::encloses,
		"has_area", &Rect2i::has_area,
		"intersection", &Rect2i::intersection,
		"merge", &Rect2i::merge,
		"has_point", &Rect2i::has_point,
		"grow", &Rect2i::grow,
		"grow_side", &Rect2i::grow_side,
		"grow_individual", &Rect2i::grow_individual,
		"expand", &Rect2i::expand,
		"expand_to", &Rect2i::expand_to,
		"abs", &Rect2i::abs,
		// cast methods
		"to_string", &Rect2i::operator String,
		"to_rect2", &Rect2i::operator Rect2,
		// operators
		sol::meta_function::to_string, &Rect2i::operator String
	);
}

}

