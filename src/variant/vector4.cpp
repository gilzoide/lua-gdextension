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

#include "vector_helpers.hpp"
#include "../constants.hpp"

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector4.hpp>
#include <godot_cpp/variant/vector4i.hpp>

using namespace godot;

namespace luagdextension {

void register_vector4(sol::state_view& state) {
	state.new_usertype<Vector4>(
		"Vector4",
		// constructors
		sol::call_constructor, sol::constructors<
			Vector4(),
			Vector4(real_t, real_t, real_t, real_t)
		>(),
		// properties
		"x", &Vector4::x,  // (x, y, z, w)
		"y", &Vector4::y,
		"z", &Vector4::z,
		"w", &Vector4::w,
		"r", &Vector4::x,  // (r, g, b, a)
		"g", &Vector4::y,
		"b", &Vector4::z,
		"a", &Vector4::w,
		"s", &Vector4::x,  // (s, t, p, q)
		"t", &Vector4::y,
		"p", &Vector4::z,
		"q", &Vector4::w,
		// constants
		"AXIS_X", sol::var(Vector4::AXIS_X),
		"AXIS_Y", sol::var(Vector4::AXIS_Y),
		"AXIS_Z", sol::var(Vector4::AXIS_Z),
		"AXIS_W", sol::var(Vector4::AXIS_W),
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::VECTOR4),
		// methods
		"min_axis_index", &Vector4::min_axis_index,
		"min_axis", &vector_min_axis<Vector4>,
		"max_axis_index", &Vector4::max_axis_index,
		"max_axis", &vector_max_axis<Vector4>,
		"length_squared", &Vector4::length_squared,
		"is_equal_approx", &Vector4::is_equal_approx,
		"is_zero_approx", &Vector4::is_zero_approx,
		"length", &Vector4::length,
		"normalize", &Vector4::normalize,
		"normalized", &Vector4::normalized,
		"is_normalized", &Vector4::is_normalized,
		"distance_to", &Vector4::distance_to,
		"distance_squared_to", &Vector4::distance_squared_to,
		"direction_to", &Vector4::direction_to,
		"abs", &Vector4::abs,
		"sign", &Vector4::sign,
		"floor", &Vector4::floor,
		"ceil", &Vector4::ceil,
		"round", &Vector4::round,
		"lerp", &Vector4::lerp,
		"cubic_interpolate", &Vector4::cubic_interpolate,
		"cubic_interpolate_in_time", &Vector4::cubic_interpolate_in_time,
		"posmod", &Vector4::posmod,
		"posmodv", &Vector4::posmodv,
		"snap", &Vector4::snap,
		"snapped", &Vector4::snapped,
		"clamp", &Vector4::clamp,
		"inverse", &Vector4::inverse,
		"dot", &Vector4::dot,
		// cast methods
		"to_string", &Vector4::operator String,
		"to_vector4i", [](const Vector4& v) { return Vector4i(v); },
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector4::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector4(const Vector4&) const>(&Vector4::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector4() const>(&Vector4::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector4(const Vector4&) const>(&Vector4::operator *),
			sol::resolve<Vector4(const real_t&) const>(&Vector4::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector4(const Vector4&) const>(&Vector4::operator /),
			sol::resolve<Vector4(const real_t&) const>(&Vector4::operator /)
		),
		sol::meta_function::to_string, &Vector4::operator String
	);
}

}

