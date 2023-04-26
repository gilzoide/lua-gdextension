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

#include <godot_cpp/variant/plane.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

void register_plane(sol::state_view& state) {
	state.new_usertype<Plane>(
		"Plane",
		// constructors
		sol::call_constructor, sol::constructors<Plane(), Plane(real_t, real_t, real_t, real_t), Plane(const Vector3&, real_t), Plane(const Vector3&, const Vector3&), Plane(const Vector3&, const Vector3&, const Vector3&)>(),
		// properties
		"normal", &Plane::normal,
		"d", &Plane::d,
		// constants
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::PLANE),
		// methods
		"normalize", &Plane::normalize,
		"normalized", &Plane::normalized,
		"get_center", &Plane::center,
		"get_any_perpendicular_normal", &Plane::get_any_perpendicular_normal,
		"is_point_over", &Plane::is_point_over,
		"distance_to", &Plane::distance_to,
		"has_point", &Plane::has_point,
		"intersect_3", &Plane::intersect_3_bind,
		"intersects_ray", &Plane::intersects_ray_bind,
		"intersects_segment", &Plane::intersects_segment_bind,
		"project", &Plane::project,
		"is_equal_approx", &Plane::is_equal_approx,
		"is_equal_approx_any_side", &Plane::is_equal_approx_any_side,
		// cast methods
		"to_string", &Plane::operator String,
		// operators
		sol::meta_function::unary_minus, &Plane::operator -,
		sol::meta_function::to_string, &Plane::operator String
	);
}

}
