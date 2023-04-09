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

#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector2i.hpp>

using namespace godot;

namespace luagdextension {

void register_vector2(sol::state_view& state) {
	state.new_usertype<Vector2>(
		"Vector2",
		// constructors
		sol::call_constructor, sol::constructors<Vector2(), Vector2(real_t, real_t)>(),
		// properties
		"x", &Vector2::x,  // (x, y)
		"y", &Vector2::y,
		"width", &Vector2::x,  // (width, height)
		"height", &Vector2::y,
		"r", &Vector2::x,  // (r, g)
		"g", &Vector2::y,
		"s", &Vector2::x,  // (s, t)
		"t", &Vector2::y,
		"u", &Vector2::x,  // (u, v)
		"v", &Vector2::y,
		// constants
		"AXIS_X", sol::var(Vector2::AXIS_X),
		"AXIS_Y", sol::var(Vector2::AXIS_Y),
		// methods
		"min_axis_index", &Vector2::min_axis_index,
		"min_axis", &vector_min_axis<Vector2>,
		"max_axis_index", &Vector2::max_axis_index,
		"max_axis", &vector_max_axis<Vector2>,
		"normalize", &Vector2::normalize,
		"normalized", &Vector2::normalized,
		"is_normalized", &Vector2::is_normalized,
		"length", &Vector2::length,
		"length_squared", &Vector2::length_squared,
		"limit_length", &Vector2::limit_length,
		"min", &Vector2::min,
		"max", &Vector2::max,
		"distance_to", &Vector2::distance_to,
		"distance_squared_to", &Vector2::distance_squared_to,
		"angle_to", &Vector2::angle_to,
		"angle_to_point", &Vector2::angle_to_point,
		"direction_to", &Vector2::direction_to,
		"dot", &Vector2::dot,
		"cross", &Vector2::cross,
		"posmod", &Vector2::posmod,
		"posmodv", &Vector2::posmodv,
		"project", &Vector2::project,
		"plane_project", &Vector2::plane_project,
		"lerp", &Vector2::lerp,
		"slerp", &Vector2::slerp,
		"cubic_interpolate", &Vector2::cubic_interpolate,
		"cubic_interpolate_in_time", &Vector2::cubic_interpolate_in_time,
		"bezier_interpolate", &Vector2::bezier_interpolate,
		"move_toward", &Vector2::move_toward,
		"slide", &Vector2::slide,
		"bounce", &Vector2::bounce,
		"reflect", &Vector2::reflect,
		"is_equal_approx", &Vector2::is_equal_approx,
		"is_zero_approx", &Vector2::is_zero_approx,
		"angle", &Vector2::angle,
		"from_angle", &Vector2::from_angle,
		"abs", &Vector2::abs,
		"rotated", &Vector2::rotated,
		"orthogonal", &Vector2::orthogonal,
		"sign", &Vector2::sign,
		"floor", &Vector2::floor,
		"ceil", &Vector2::ceil,
		"round", &Vector2::round,
		"snapped", &Vector2::snapped,
		"clamp", &Vector2::clamp,
		"aspect", &Vector2::aspect,
		// cast methods
		"to_string", &Vector2::operator String,
		"to_vector2i", &Vector2::operator Vector2i,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector2::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector2(const Vector2&) const>(&Vector2::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector2() const>(&Vector2::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector2(const Vector2&) const>(&Vector2::operator *),
			sol::resolve<Vector2(const real_t&) const>(&Vector2::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector2(const Vector2&) const>(&Vector2::operator /),
			sol::resolve<Vector2(const real_t&) const>(&Vector2::operator /)
		),
		sol::meta_function::to_string, &Vector2::operator String
	);
}

}
