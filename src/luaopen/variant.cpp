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

#include "../godot_utils.hpp"
#include "../lua_utils.hpp"

using namespace godot;

namespace luagdextension {

template<Variant::Type VarType>
Variant construct_variant_from_lua(const sol::variadic_args& args) {
	VariantArguments variant_args = args;

	Variant result;
	GDExtensionCallError error;
	internal::gdextension_interface_variant_construct((GDExtensionVariantType) VarType, &result, (GDExtensionConstVariantPtr *) variant_args.argv(), variant_args.argc(), &error);
	if (error.error != GDEXTENSION_CALL_OK) {
		UtilityFunctions::printerr("TODO: CONSTRUCT ERROR");
	}
	return result;
}

template<Variant::Operator VarOperator>
Variant evaluate_operator(const sol::stack_object& a, const sol::stack_object& b) {
	bool is_valid;
	Variant result;
	Variant::evaluate(VarOperator, to_variant(a), to_variant(b), result, is_valid);
	if (!is_valid) {
		UtilityFunctions::printerr("TODO: OPERATOR ERROR");
	}
	return result;
}

sol::stack_object variant_index(const Variant& variant, const sol::stack_object& key) {
	lua_State *L = key.lua_state();
	if (key.get_type() == sol::type::string) {
		StringName string_name = key.as<StringName>();
		if (variant.has_method(string_name)) {
			sol::stack::push(L, MethodBindByName(string_name));
			return sol::stack_object(L, -1);
		}
	}

	bool is_valid;
	Variant result = variant.get(to_variant(key), &is_valid);
	if (!is_valid) {
		UtilityFunctions::printerr("TODO: INDEX ERROR ", to_variant(key));
	}
	return to_lua(L, result);
}

void variant_newindex(Variant& variant, const sol::stack_object& key, const sol::stack_object& value) {
	bool is_valid;
	variant.set(to_variant(key), to_variant(value), &is_valid);
	if (!is_valid) {
		UtilityFunctions::printerr("TODO: NEWINDEX ERROR ", to_variant(key));
	}
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
		// comparison
		sol::meta_function::equal_to, &evaluate_operator<Variant::OP_EQUAL>,
		sol::meta_function::less_than, &evaluate_operator<Variant::OP_LESS>,
		sol::meta_function::less_than_or_equal_to, &evaluate_operator<Variant::OP_LESS_EQUAL>,
		// mathematic
		sol::meta_function::addition, &evaluate_operator<Variant::OP_ADD>,
		sol::meta_function::subtraction, &evaluate_operator<Variant::OP_SUBTRACT>,
		sol::meta_function::multiplication, &evaluate_operator<Variant::OP_MULTIPLY>,
		sol::meta_function::division, &evaluate_operator<Variant::OP_DIVIDE>,
		sol::meta_function::unary_minus, &evaluate_operator<Variant::OP_NEGATE>,
		sol::meta_function::modulus, &evaluate_operator<Variant::OP_MODULE>,
		// bitwise
		sol::meta_function::bitwise_left_shift, &evaluate_operator<Variant::OP_SHIFT_LEFT>,
		sol::meta_function::bitwise_right_shift, &evaluate_operator<Variant::OP_SHIFT_RIGHT>,
		sol::meta_function::bitwise_and, &evaluate_operator<Variant::OP_BIT_AND>,
		sol::meta_function::bitwise_or, &evaluate_operator<Variant::OP_BIT_OR>,
		sol::meta_function::bitwise_xor, &evaluate_operator<Variant::OP_BIT_XOR>,
		sol::meta_function::bitwise_not, &evaluate_operator<Variant::OP_BIT_NEGATE>,
		// misc
		sol::meta_function::index, &variant_index,
		sol::meta_function::new_index, &variant_newindex,
		sol::meta_function::to_string, &Variant::operator String
	);

	state.new_usertype<MethodBindByName>(
		"MethodBindByName",
		sol::meta_function::call, &MethodBindByName::call,
		sol::meta_function::to_string, &MethodBindByName::get_method_name
	);

	// math types
	state.set_function("Vector2", &construct_variant_from_lua<Variant::VECTOR2>);
	state.set_function("Vector2i", &construct_variant_from_lua<Variant::VECTOR2I>);
	state.set_function("Rect2", &construct_variant_from_lua<Variant::RECT2>);
	state.set_function("Rect2i", &construct_variant_from_lua<Variant::RECT2I>);
	state.set_function("Vector3", &construct_variant_from_lua<Variant::VECTOR3>);
	state.set_function("Vector3i", &construct_variant_from_lua<Variant::VECTOR3I>);
	state.set_function("Transform2D", &construct_variant_from_lua<Variant::TRANSFORM2D>);
	state.set_function("Vector4", &construct_variant_from_lua<Variant::VECTOR4>);
	state.set_function("Vector4i", &construct_variant_from_lua<Variant::VECTOR4I>);
	state.set_function("Plane", &construct_variant_from_lua<Variant::PLANE>);
	state.set_function("Quaternion", &construct_variant_from_lua<Variant::QUATERNION>);
	state.set_function("AABB", &construct_variant_from_lua<Variant::AABB>);
	state.set_function("Basis", &construct_variant_from_lua<Variant::BASIS>);
	state.set_function("Transform3D", &construct_variant_from_lua<Variant::TRANSFORM3D>);
	state.set_function("Projection", &construct_variant_from_lua<Variant::PROJECTION>);

	// misc types
	state.set_function("Color", &construct_variant_from_lua<Variant::COLOR>);
	state.set_function("StringName", &construct_variant_from_lua<Variant::STRING_NAME>);
	state.set_function("NodePath", &construct_variant_from_lua<Variant::NODE_PATH>);
	state.set_function("RID", &construct_variant_from_lua<Variant::RID>);
	state.set_function("Callable", &construct_variant_from_lua<Variant::CALLABLE>);
	state.set_function("Signal", &construct_variant_from_lua<Variant::SIGNAL>);
	state.set_function("Dictionary", &construct_variant_from_lua<Variant::DICTIONARY>);
	state.set_function("Array", &construct_variant_from_lua<Variant::ARRAY>);

	// typed arrays
	state.set_function("PackedByteArray", &construct_variant_from_lua<Variant::PACKED_BYTE_ARRAY>);
	state.set_function("PackedInt32Array", &construct_variant_from_lua<Variant::PACKED_INT32_ARRAY>);
	state.set_function("PackedInt64Array", &construct_variant_from_lua<Variant::PACKED_INT64_ARRAY>);
	state.set_function("PackedFloat32Array", &construct_variant_from_lua<Variant::PACKED_FLOAT32_ARRAY>);
	state.set_function("PackedFloat64Array", &construct_variant_from_lua<Variant::PACKED_FLOAT64_ARRAY>);
	state.set_function("PackedStringArray", &construct_variant_from_lua<Variant::PACKED_STRING_ARRAY>);
	state.set_function("PackedVector2Array", &construct_variant_from_lua<Variant::PACKED_VECTOR2_ARRAY>);
	state.set_function("PackedVector3Array", &construct_variant_from_lua<Variant::PACKED_VECTOR3_ARRAY>);
	state.set_function("PackedColorArray", &construct_variant_from_lua<Variant::PACKED_COLOR_ARRAY>);

	return 0;
}

