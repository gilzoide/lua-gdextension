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

#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

void register_quaternion(sol::state_view& state) {
	state.new_usertype<Quaternion>(
		"Quaternion",
		// constructors
		sol::call_constructor, sol::constructors<
			Quaternion(),
			Quaternion(real_t, real_t, real_t, real_t),
			Quaternion(const Vector3&, real_t),
			Quaternion(const Vector3&),
			Quaternion(const Quaternion&),
			Quaternion(const Vector3&, const Vector3&)
		>(),
		// properties
		"x", &Quaternion::x,
		"y", &Quaternion::y,
		"z", &Quaternion::z,
		"w", &Quaternion::w,
		// constants
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::QUATERNION),
		// methods
		"length_squared", &Quaternion::length_squared,
		"is_equal_approx", &Quaternion::is_equal_approx,
		"length", &Quaternion::length,
		"normalize", &Quaternion::normalize,
		"is_normalized", &Quaternion::is_normalized,
		"inverse", &Quaternion::inverse,
		"log", &Quaternion::log,
		"exp", &Quaternion::exp,
		"dot", &Quaternion::dot,
		"angle_to", &Quaternion::angle_to,
		"get_euler_xyz", &Quaternion::get_euler_xyz,
		"get_euler_yxz", &Quaternion::get_euler_yxz,
		"get_euler", &Quaternion::get_euler,
		"slerp", &Quaternion::slerp,
		"slerpni", &Quaternion::slerpni,
		"spherical_cubic_interpolate", &Quaternion::spherical_cubic_interpolate,
		"spherical_cubic_interpolate_in_time", &Quaternion::spherical_cubic_interpolate_in_time,
		"get_axis", &Quaternion::get_axis,
		"get_angle", &Quaternion::get_angle,
		"get_axis_angle", [](const Quaternion& quat) {
			Vector3 axis;
			real_t angle;
			quat.get_axis_angle(axis, angle);
			return std::make_tuple(axis, angle);
		},
		"xform", &Quaternion::xform,
		"xform_inv", &Quaternion::xform_inv,
		// cast methods
		"to_string", &Quaternion::operator String,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Quaternion::operator +,
		sol::meta_function::subtraction, sol::resolve<Quaternion(const Quaternion&) const>(&Quaternion::operator -),
		sol::meta_function::unary_minus, sol::resolve<Quaternion() const>(&Quaternion::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Quaternion(const Quaternion&) const>(&Quaternion::operator *),
			sol::resolve<Quaternion(const real_t&) const>(&Quaternion::operator *)
		),
		sol::meta_function::division, &Quaternion::operator /,
		sol::meta_function::to_string, &Quaternion::operator String
	);
}

}

