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

void register_vector4i(sol::state_view& state) {
	state.new_usertype<Vector4i>(
		"Vector4i",
		// constructors
		sol::call_constructor, sol::constructors<Vector4i(), Vector4i(const Vector4&), Vector4i(const int32_t, const int32_t, const int32_t, const int32_t)>(),
		// properties
		"x", &Vector4i::x,  // (x, y, z, w)
		"y", &Vector4i::y,
		"z", &Vector4i::z,
		"w", &Vector4i::w,
		// constants
		"AXIS_X", sol::var(Vector4i::AXIS_X),
		"AXIS_Y", sol::var(Vector4i::AXIS_Y),
		"AXIS_Z", sol::var(Vector4i::AXIS_Z),
		"AXIS_W", sol::var(Vector4i::AXIS_W),
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::VECTOR4I),
		// methods
		"min_axis_index", &Vector4i::min_axis_index,
		"min_axis", &vector_min_axis<Vector4i>,
		"max_axis_index", &Vector4i::max_axis_index,
		"max_axis", &vector_max_axis<Vector4i>,
		"length_squared", &Vector4i::length_squared,
		"length", &Vector4i::length,
		"zero", &Vector4i::zero,
		"abs", &Vector4i::abs,
		"sign", &Vector4i::sign,
		"clamp", &Vector4i::clamp,
		// cast methods
		"to_string", &Vector4i::operator String,
		"to_vector4", &Vector4i::operator Vector4,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector4i::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector4i(const Vector4i&) const>(&Vector4i::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector4i() const>(&Vector4i::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector4i(const Vector4i&) const>(&Vector4i::operator *),
			sol::resolve<Vector4i(const int32_t) const>(&Vector4i::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector4i(const Vector4i&) const>(&Vector4i::operator /),
			sol::resolve<Vector4i(const int32_t) const>(&Vector4i::operator /)
		),
		sol::meta_function::modulus, sol::overload(
			sol::resolve<Vector4i(const Vector4i&) const>(&Vector4i::operator %),
			sol::resolve<Vector4i(const int32_t) const>(&Vector4i::operator %)
		),
		sol::meta_function::to_string, &Vector4i::operator String
	);
}

}


