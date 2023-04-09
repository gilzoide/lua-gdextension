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

#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector3i.hpp>

using namespace godot;

namespace luagdextension {

void register_vector3(sol::state_view& state) {
	state.new_usertype<Vector3>(
		"Vector3",
		// constructors
		sol::call_constructor, sol::constructors<Vector3(), Vector3(real_t, real_t, real_t)>(),
		// properties
		"x", &Vector3::x,  // (x, y, z)
		"y", &Vector3::y,
		"z", &Vector3::z,
		"width", &Vector3::x,  // (width, height, depth)
		"height", &Vector3::y,
		"depth", &Vector3::z,
		"r", &Vector3::x,  // (r, g, b)
		"g", &Vector3::y,
		"b", &Vector3::z,
		"s", &Vector3::x,  // (s, t, p)
		"t", &Vector3::y,
		"p", &Vector3::z,
		// constants
		"AXIS_X", sol::var(Vector3::AXIS_X),
		"AXIS_Y", sol::var(Vector3::AXIS_Y),
		"AXIS_Z", sol::var(Vector3::AXIS_Z),
		// methods
		"min_axis_index", &Vector3::min_axis_index,
		"min_axis", &vector_min_axis<Vector3>,
		"max_axis_index", &Vector3::max_axis_index,
		"max_axis", &vector_max_axis<Vector3>,
		"length", &Vector3::length,
		"length_squared", &Vector3::length_squared,
		"normalize", &Vector3::normalize,
		"normalized", &Vector3::normalized,
		"is_normalized", &Vector3::is_normalized,
		"inverse", &Vector3::inverse,
		"limit_length", &Vector3::limit_length,
		"zero", &Vector3::zero,
		"snap", &Vector3::snap,
		"snapped", &Vector3::snapped,
		"rotate", &Vector3::rotate,
		"rotated", &Vector3::rotated,
		"lerp", &Vector3::lerp,
		"slerp", &Vector3::slerp,
		"cubic_interpolate", &Vector3::cubic_interpolate,
		"cubic_interpolate_in_time", &Vector3::cubic_interpolate_in_time,
		"bezier_interpolate", &Vector3::bezier_interpolate,
		"move_toward", &Vector3::move_toward,
		"octahedron_encode", &Vector3::octahedron_encode,
		"octahedron_decode", &Vector3::octahedron_decode,
		"octahedron_tangent_encode", &Vector3::octahedron_tangent_encode,
		"octahedron_tangent_decode", &Vector3::octahedron_tangent_decode,
		"cross", &Vector3::cross,
		"dot", &Vector3::dot,
		"outer", &Vector3::outer,
		"abs", &Vector3::abs,
		"floor", &Vector3::floor,
		"sign", &Vector3::sign,
		"ceil", &Vector3::ceil,
		"round", &Vector3::round,
		"clamp", &Vector3::clamp,
		"distance_to", &Vector3::distance_to,
		"distance_squared_to", &Vector3::distance_squared_to,
		"posmod", &Vector3::posmod,
		"posmodv", &Vector3::posmodv,
		"project", &Vector3::project,
		"angle_to", &Vector3::angle_to,
		"signed_angle_to", &Vector3::signed_angle_to,
		"direction_to", &Vector3::direction_to,
		"slide", &Vector3::slide,
		"bounce", &Vector3::bounce,
		"reflect", &Vector3::reflect,
		"is_equal_approx", &Vector3::is_equal_approx,
		"is_zero_approx", &Vector3::is_zero_approx,
		// cast methods
		"to_string", &Vector3::operator String,
		"to_vector3i", &Vector3::operator Vector3i,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector3::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector3(const Vector3&) const>(&Vector3::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector3() const>(&Vector3::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector3(const Vector3&) const>(&Vector3::operator *),
			sol::resolve<Vector3(const real_t) const>(&Vector3::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector3(const Vector3&) const>(&Vector3::operator /),
			sol::resolve<Vector3(const real_t) const>(&Vector3::operator /)
		),
		sol::meta_function::to_string, &Vector3::operator String
	);
}

}

