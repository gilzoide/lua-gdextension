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

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector3i.hpp>

using namespace godot;

namespace luagdextension {

void register_vector3i(sol::state_view& state) {
	state.new_usertype<Vector3i>(
		"Vector3i",
		// constructors
		sol::call_constructor, sol::constructors<Vector3i(), Vector3i(const int32_t, const int32_t, const int32_t)>(),
		// properties
		"x", &Vector3i::x,  // (x, y, z)
		"y", &Vector3i::y,
		"z", &Vector3i::z,
		"width", &Vector3i::x,  // (width, height, depth)
		"height", &Vector3i::y,
		"depth", &Vector3i::z,
		// constants
		"AXIS_X", sol::var(Vector3i::AXIS_X),
		"AXIS_Y", sol::var(Vector3i::AXIS_Y),
		"AXIS_Z", sol::var(Vector3i::AXIS_Z),
		// methods
		"min_axis_index", &Vector3i::min_axis_index,
		"min_axis", &vector_min_axis<Vector3i>,
		"max_axis_index", &Vector3i::max_axis_index,
		"max_axis", &vector_max_axis<Vector3i>,
		"length_squared", &Vector3i::length_squared,
		"length", &Vector3i::length,
		"zero", &Vector3i::zero,
		"abs", &Vector3i::abs,
		"sign", &Vector3i::sign,
		"clamp", &Vector3i::clamp,
		// cast methods
		"to_string", &Vector3i::operator String,
		"to_vector3", &Vector3i::operator Vector3,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector3i::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector3i(const Vector3i&) const>(&Vector3i::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector3i() const>(&Vector3i::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector3i(const Vector3i&) const>(&Vector3i::operator *),
			sol::resolve<Vector3i(const int32_t) const>(&Vector3i::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector3i(const Vector3i&) const>(&Vector3i::operator /),
			sol::resolve<Vector3i(const int32_t) const>(&Vector3i::operator /)
		),
		sol::meta_function::modulus, sol::overload(
			sol::resolve<Vector3i(const Vector3i&) const>(&Vector3i::operator %),
			sol::resolve<Vector3i(const int32_t) const>(&Vector3i::operator %)
		),
		sol::meta_function::to_string, &to_lua_string<Vector3i>
	);
}

}


