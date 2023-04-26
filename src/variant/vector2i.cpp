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

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector2i.hpp>

using namespace godot;

namespace luagdextension {

void register_vector2i(sol::state_view& state) {
	state.new_usertype<Vector2i>(
		"Vector2i",
		// constructors
		sol::call_constructor, sol::constructors<Vector2i(), Vector2(const int32_t, const int32_t)>(),
		// properties
		"x", &Vector2i::x,  // (x, y)
		"y", &Vector2i::y,
		"width", &Vector2i::x,  // (width, height)
		"height", &Vector2i::y,
		// constants
		"AXIS_X", sol::var(Vector2i::AXIS_X),
		"AXIS_Y", sol::var(Vector2i::AXIS_Y),
		LUA_META_VARIANT_TYPE, sol::var(Variant::Type::VECTOR2I),
		// methods
		"min_axis_index", &Vector2i::min_axis_index,
		"min_axis", &vector_min_axis<Vector2i>,
		"max_axis_index", &Vector2i::max_axis_index,
		"max_axis", &vector_max_axis<Vector2i>,
		"min", &Vector2i::min,
		"max", &Vector2i::max,
		"length_squared", &Vector2i::length_squared,
		"length", &Vector2i::length,
		"aspect", &Vector2i::aspect,
		"sign", &Vector2i::sign,
		"abs", &Vector2i::abs,
		"clamp", &Vector2i::clamp,
		// cast methods
		"to_string", &Vector2i::operator String,
		"to_vector2", &Vector2i::operator Vector2,
		// operators (logical operators are automatically bound by sol)
		sol::meta_function::addition, &Vector2i::operator +,
		sol::meta_function::subtraction, sol::resolve<Vector2i(const Vector2i&) const>(&Vector2i::operator -),
		sol::meta_function::unary_minus, sol::resolve<Vector2i() const>(&Vector2i::operator -),
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<Vector2i(const Vector2i&) const>(&Vector2i::operator *),
			sol::resolve<Vector2i(const int32_t&) const>(&Vector2i::operator *)
		),
		sol::meta_function::division, sol::overload(
			sol::resolve<Vector2i(const Vector2i&) const>(&Vector2i::operator /),
			sol::resolve<Vector2i(const int32_t&) const>(&Vector2i::operator /)
		),
		sol::meta_function::modulus, sol::overload(
			sol::resolve<Vector2i(const Vector2i&) const>(&Vector2i::operator %),
			sol::resolve<Vector2i(const int32_t&) const>(&Vector2i::operator %)
		),
		sol::meta_function::to_string, &Vector2i::operator String
	);
}

}

