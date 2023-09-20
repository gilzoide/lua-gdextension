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

#include <sol/sol.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../utils/convert_godot_lua.hpp"
#include "../utils/convert_godot_std.hpp"
#include "../utils/VariantArguments.hpp"
#include "../utils/VariantClass.hpp"
#include "../utils/VariantClassMethod.hpp"
#include "../utils/MethodBindByName.hpp"

using namespace godot;

namespace luagdextension {

template<Variant::Operator VarOperator>
Variant evaluate_binary_operator(const sol::stack_object& a, const sol::stack_object& b) {
	bool is_valid;
	Variant result;
	Variant::evaluate(VarOperator, to_variant(a), to_variant(b), result, is_valid);
	if (!is_valid) {
		luaL_error(a.lua_state(), "Error in operator. TODO: format error information");
	}
	return result;
}

template<Variant::Operator VarOperator>
Variant evaluate_unary_operator(const sol::stack_object& a) {
	bool is_valid;
	Variant result;
	Variant::evaluate(VarOperator, to_variant(a), Variant(), result, is_valid);
	if (!is_valid) {
		luaL_error(a.lua_state(), "Error in operator. TODO: format error information");
	}
	return result;
}

sol::stack_object variant_index(const Variant& variant, const sol::stack_object& key) {
	bool is_valid;
	lua_State *L = key.lua_state();
	if (key.get_type() == sol::type::string) {
		StringName string_name = key.as<StringName>();
		if (Variant::has_member(variant.get_type(), string_name)) {
			return to_lua(L, variant.get_named(string_name, is_valid));
		}
		else if (variant.has_method(string_name)) {
			sol::stack::push(L, MethodBindByName(string_name));
			return sol::stack_object(L, -1);
		}
	}

	Variant result = variant.get(to_variant(key), &is_valid);
	if (!is_valid) {
		luaL_error(L, "Error in __index. TODO: format error information");
	}
	return to_lua(L, result);
}

void variant_newindex(Variant& variant, const sol::stack_object& key, const sol::stack_object& value) {
	bool is_valid;
	variant.set(to_variant(key), to_variant(value), &is_valid);
	if (!is_valid) {
		luaL_error(key.lua_state(), "Error in __newindex. TODO: format error information");
	}
}

bool variant_is(const Variant& a, const VariantClass& cls) {
	return a.get_type() == cls.get_type();
}

}

using namespace luagdextension;

extern "C" int luaopen_godot_variant(lua_State *L) {
	sol::state_view state = L;

	state.new_usertype<Variant>(
		"Variant",
		sol::call_constructor, sol::constructors<
			Variant(),
			Variant(const Variant &other),
			Variant(Variant &&other),
			Variant(bool v),
			Variant(int64_t v),
			Variant(float v),
			Variant(double v),
			Variant(const char *v)
		>(),
		"is", &variant_is,
		// comparison
		sol::meta_function::equal_to, &evaluate_binary_operator<Variant::OP_EQUAL>,
		sol::meta_function::less_than, &evaluate_binary_operator<Variant::OP_LESS>,
		sol::meta_function::less_than_or_equal_to, &evaluate_binary_operator<Variant::OP_LESS_EQUAL>,
		// mathematic
		sol::meta_function::addition, &evaluate_binary_operator<Variant::OP_ADD>,
		sol::meta_function::subtraction, &evaluate_binary_operator<Variant::OP_SUBTRACT>,
		sol::meta_function::multiplication, &evaluate_binary_operator<Variant::OP_MULTIPLY>,
		sol::meta_function::division, &evaluate_binary_operator<Variant::OP_DIVIDE>,
		sol::meta_function::unary_minus, &evaluate_unary_operator<Variant::OP_NEGATE>,
		sol::meta_function::modulus, &evaluate_binary_operator<Variant::OP_MODULE>,
		// bitwise
		sol::meta_function::bitwise_left_shift, &evaluate_binary_operator<Variant::OP_SHIFT_LEFT>,
		sol::meta_function::bitwise_right_shift, &evaluate_binary_operator<Variant::OP_SHIFT_RIGHT>,
		sol::meta_function::bitwise_and, &evaluate_binary_operator<Variant::OP_BIT_AND>,
		sol::meta_function::bitwise_or, &evaluate_binary_operator<Variant::OP_BIT_OR>,
		sol::meta_function::bitwise_xor, &evaluate_binary_operator<Variant::OP_BIT_XOR>,
		sol::meta_function::bitwise_not, &evaluate_unary_operator<Variant::OP_BIT_NEGATE>,
		// misc
		sol::meta_function::index, &variant_index,
		sol::meta_function::new_index, &variant_newindex,
		sol::meta_function::to_string, &Variant::operator String
	);

	MethodBindByName::register_usertype(state);
	VariantClass::register_usertype(state);
	VariantClassMethod::register_usertype(state);

	// atomic types
	state.set("bool", VariantClass(Variant::BOOL));
	state.set("int", VariantClass(Variant::INT));
	state.set("float", VariantClass(Variant::FLOAT));
	state.set("String", VariantClass(Variant::STRING));

	// math types
	state.set("Vector2", VariantClass(Variant::VECTOR2));
	state.set("Vector2i", VariantClass(Variant::VECTOR2I));
	state.set("Rect2", VariantClass(Variant::RECT2));
	state.set("Rect2i", VariantClass(Variant::RECT2I));
	state.set("Vector3", VariantClass(Variant::VECTOR3));
	state.set("Vector3i", VariantClass(Variant::VECTOR3I));
	state.set("Transform2D", VariantClass(Variant::TRANSFORM2D));
	state.set("Vector4", VariantClass(Variant::VECTOR4));
	state.set("Vector4i", VariantClass(Variant::VECTOR4I));
	state.set("Plane", VariantClass(Variant::PLANE));
	state.set("Quaternion", VariantClass(Variant::QUATERNION));
	state.set("AABB", VariantClass(Variant::AABB));
	state.set("Basis", VariantClass(Variant::BASIS));
	state.set("Transform3D", VariantClass(Variant::TRANSFORM3D));
	state.set("Projection", VariantClass(Variant::PROJECTION));

	// misc types
	state.set("Color", VariantClass(Variant::COLOR));
	state.set("StringName", VariantClass(Variant::STRING_NAME));
	state.set("NodePath", VariantClass(Variant::NODE_PATH));
	state.set("RID", VariantClass(Variant::RID));
	state.set("Callable", VariantClass(Variant::CALLABLE));
	state.set("Signal", VariantClass(Variant::SIGNAL));
	state.set("Dictionary", VariantClass(Variant::DICTIONARY));
	state.set("Array", VariantClass(Variant::ARRAY));

	// typed arrays
	state.set("PackedByteArray", VariantClass(Variant::PACKED_BYTE_ARRAY));
	state.set("PackedInt32Array", VariantClass(Variant::PACKED_INT32_ARRAY));
	state.set("PackedInt64Array", VariantClass(Variant::PACKED_INT64_ARRAY));
	state.set("PackedFloat32Array", VariantClass(Variant::PACKED_FLOAT32_ARRAY));
	state.set("PackedFloat64Array", VariantClass(Variant::PACKED_FLOAT64_ARRAY));
	state.set("PackedStringArray", VariantClass(Variant::PACKED_STRING_ARRAY));
	state.set("PackedVector2Array", VariantClass(Variant::PACKED_VECTOR2_ARRAY));
	state.set("PackedVector3Array", VariantClass(Variant::PACKED_VECTOR3_ARRAY));
	state.set("PackedColorArray", VariantClass(Variant::PACKED_COLOR_ARRAY));

	return 0;
}

