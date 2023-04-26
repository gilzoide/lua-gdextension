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

#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

void register_aabb(sol::state_view& state) {
	state.new_usertype<AABB>(
		"AABB",
		// constructors
		sol::call_constructor, sol::constructors<
			AABB(),
			AABB(const Vector3&, const Vector3&)
		>(),
		// properties
		"position", &AABB::position,
		"size", &AABB::size,
		"end", sol::property(&AABB::set_end, &AABB::get_end),
		// constants
		LUA_META_VARIANT_TYPE, sol::var(Variant::AABB),
		// methods
		"get_volume", &AABB::get_volume,
		"has_volume", &AABB::has_volume,
		"has_surface", &AABB::has_surface,
		"is_equal_approx", &AABB::is_equal_approx,
		"intersects", &AABB::intersects,
		"intersects_inclusive", &AABB::intersects_inclusive,
		"encloses", &AABB::encloses,
		"merge", &AABB::merge,
		"merge_with", &AABB::merge_with,
		"intersection", &AABB::intersection,
		"intersects_segment", &AABB::intersects_segment_bind,
		"intersects_ray", &AABB::intersects_ray_bind,
		"intersects_plane", &AABB::intersects_plane,
		"has_point", &AABB::has_point,
		"get_support", &AABB::get_support,
		"get_longest_axis", &AABB::get_longest_axis,
		"get_longest_axis_index", &AABB::get_longest_axis_index,
		"get_longest_axis_size", &AABB::get_longest_axis_size,
		"get_shortest_axis", &AABB::get_shortest_axis,
		"get_shortest_axis_index", &AABB::get_shortest_axis_index,
		"get_shortest_axis_size", &AABB::get_shortest_axis_size,
		"grow", &AABB::grow,
		"grow_by", &AABB::grow_by,
		"get_endpoint", &AABB::get_endpoint,
		"expand", &AABB::expand,
		"expand_to", &AABB::expand_to,
		"abs", &AABB::abs,
		"quantize", &AABB::quantize,
		"quantized", &AABB::quantized,
		"get_center", &AABB::get_center,
		// cast methods
		"to_string", &AABB::operator String,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::to_string, &AABB::operator String
	);
}

}
