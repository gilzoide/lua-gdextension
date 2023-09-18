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

#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

template<int axis>
Vector3 get_axis(const Basis& basis) {
	return basis.rows[axis];
}

template<int axis>
void set_axis(Basis& basis, const Vector3& row) {
	basis.rows[axis] = row;
}

void register_basis(sol::state_view& state) {
	state.new_usertype<Basis>(
		"Basis",
		// constructors
		sol::call_constructor, sol::constructors<
			Basis(),
			Basis(const Vector3&, real_t),
			Basis(const Quaternion&),
			Basis(const Vector3&, const Vector3&, const Vector3&)
		>(),
		// properties
		"x", sol::property(&set_axis<0>, &get_axis<0>),
		"y", sol::property(&set_axis<1>, &get_axis<1>),
		"z", sol::property(&set_axis<2>, &get_axis<2>),
		// constants
		LUA_META_VARIANT_TYPE, sol::var(Variant::BASIS),
		// methods
		"invert", &Basis::invert,
		"transpose", &Basis::transpose,
		"inverse", &Basis::inverse,
		"transposed", &Basis::transposed,
		"determinant", &Basis::determinant,
		"rotate", sol::overload(
			sol::resolve<void (const Vector3&, real_t)>(&Basis::rotate),
			sol::resolve<void (const Quaternion&)>(&Basis::rotate)
		),
		"rotated", sol::overload(
			sol::resolve<Basis (const Vector3&, real_t) const>(&Basis::rotated),
			sol::resolve<Basis (const Quaternion&) const>(&Basis::rotated)
		),
		"rotate_local", &Basis::rotate_local,
		"rotated_local", &Basis::rotated_local,
		"get_euler_normalized", &Basis::get_euler_normalized,
		"get_rotation_quaternion", &Basis::get_rotation_quaternion,
		"get_euler", &Basis::get_euler,
		"from_euler", &Basis::from_euler,
		"scale", &Basis::scale,
		"scaled", &Basis::scaled,
		"scale_local", &Basis::scale_local,
		"scaled_local", &Basis::scaled_local,
		"scale_orthogonal", &Basis::scale_orthogonal,
		"scaled_orthogonal", &Basis::scaled_orthogonal,
		"get_scale", &Basis::get_scale,
		"get_scale_abs", &Basis::get_scale_abs,
		"get_scale_local", &Basis::get_scale_local,
		"tdotx", &Basis::tdotx,
		"tdoty", &Basis::tdoty,
		"tdotz", &Basis::tdotz,
		"is_equal_approx", &Basis::is_equal_approx,
		"xform", &Basis::xform,
		"xform_inv", &Basis::xform_inv,
		"is_orthogonal", &Basis::is_orthogonal,
		"is_diagonal", &Basis::is_diagonal,
		"is_rotation", &Basis::is_rotation,
		"lerp", &Basis::lerp,
		"slerp", &Basis::slerp,
		"orthonormalize", &Basis::orthonormalize,
		"orthonormalized", &Basis::orthonormalized,
		// cast methods
		"to_string", &Basis::operator String,
		"to_quaternion", &Basis::operator Quaternion,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::to_string, &Basis::operator String
	);
}

}

