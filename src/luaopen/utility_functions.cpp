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

#include "godot.hpp"

#include "../utils/function_wrapper.hpp"

// We can't call variadic templates at runtime, so we need access
// to the private/internal functions accepting Variant arrays
// (what the hack?)
#define private public
#include <godot_cpp/variant/utility_functions.hpp>
#undef private

using namespace godot;
using namespace luagdextension;

extern "C" int luaopen_godot_utility_functions(lua_State *L) {
	sol::state_view state = L;

	state.set("sin", wrap_function(&UtilityFunctions::sin));
	state.set("cos", wrap_function(&UtilityFunctions::cos));
	state.set("tan", wrap_function(&UtilityFunctions::tan));
	state.set("sinh", wrap_function(&UtilityFunctions::sinh));
	state.set("cosh", wrap_function(&UtilityFunctions::cosh));
	state.set("tanh", wrap_function(&UtilityFunctions::tanh));
	state.set("asin", wrap_function(&UtilityFunctions::asin));
	state.set("acos", wrap_function(&UtilityFunctions::acos));
	state.set("atan", wrap_function(&UtilityFunctions::atan));
	state.set("atan2", wrap_function(&UtilityFunctions::atan2));
	state.set("sqrt", wrap_function(&UtilityFunctions::sqrt));
	state.set("fmod", wrap_function(&UtilityFunctions::fmod));
	state.set("fposmod", wrap_function(&UtilityFunctions::fposmod));
	state.set("posmod", wrap_function(&UtilityFunctions::posmod));
	state.set("floor", wrap_function(&UtilityFunctions::floor));
	state.set("floorf", wrap_function(&UtilityFunctions::floorf));
	state.set("floori", wrap_function(&UtilityFunctions::floori));
	state.set("ceil", wrap_function(&UtilityFunctions::ceil));
	state.set("ceilf", wrap_function(&UtilityFunctions::ceilf));
	state.set("ceili", wrap_function(&UtilityFunctions::ceili));
	state.set("round", wrap_function(&UtilityFunctions::round));
	state.set("roundf", wrap_function(&UtilityFunctions::roundf));
	state.set("roundi", wrap_function(&UtilityFunctions::roundi));
	state.set("abs", wrap_function(&UtilityFunctions::abs));
	state.set("absf", wrap_function(&UtilityFunctions::absf));
	state.set("absi", wrap_function(&UtilityFunctions::absi));
	state.set("sign", wrap_function(&UtilityFunctions::sign));
	state.set("signf", wrap_function(&UtilityFunctions::signf));
	state.set("signi", wrap_function(&UtilityFunctions::signi));
	state.set("snapped", wrap_function(&UtilityFunctions::snapped));
	state.set("snappedf", wrap_function(&UtilityFunctions::snappedf));
	state.set("snappedi", wrap_function(&UtilityFunctions::snappedi));
	state.set("pow", wrap_function(&UtilityFunctions::pow));
	state.set("log", wrap_function(&UtilityFunctions::log));
	state.set("exp", wrap_function(&UtilityFunctions::exp));
	state.set("is_nan", wrap_function(&UtilityFunctions::is_nan));
	state.set("is_inf", wrap_function(&UtilityFunctions::is_inf));
	state.set("is_equal_approx", wrap_function(&UtilityFunctions::is_equal_approx));
	state.set("is_zero_approx", wrap_function(&UtilityFunctions::is_zero_approx));
	state.set("is_finite", wrap_function(&UtilityFunctions::is_finite));
	state.set("ease", wrap_function(&UtilityFunctions::ease));
	state.set("step_decimals", wrap_function(&UtilityFunctions::step_decimals));
	state.set("lerp", wrap_function(&UtilityFunctions::lerp));
	state.set("lerpf", wrap_function(&UtilityFunctions::lerpf));
	state.set("cubic_interpolate", wrap_function(&UtilityFunctions::cubic_interpolate));
	state.set("cubic_interpolate_angle", wrap_function(&UtilityFunctions::cubic_interpolate_angle));
	state.set("cubic_interpolate_in_time", wrap_function(&UtilityFunctions::cubic_interpolate_in_time));
	state.set("cubic_interpolate_angle_in_time", wrap_function(&UtilityFunctions::cubic_interpolate_angle_in_time));
	state.set("bezier_interpolate", wrap_function(&UtilityFunctions::bezier_interpolate));
	state.set("bezier_derivative", wrap_function(&UtilityFunctions::bezier_derivative));
	state.set("lerp_angle", wrap_function(&UtilityFunctions::lerp_angle));
	state.set("inverse_lerp", wrap_function(&UtilityFunctions::inverse_lerp));
	state.set("remap", wrap_function(&UtilityFunctions::remap));
	state.set("smoothstep", wrap_function(&UtilityFunctions::smoothstep));
	state.set("move_toward", wrap_function(&UtilityFunctions::move_toward));
	state.set("deg_to_rad", wrap_function(&UtilityFunctions::deg_to_rad));
	state.set("rad_to_deg", wrap_function(&UtilityFunctions::rad_to_deg));
	state.set("linear_to_db", wrap_function(&UtilityFunctions::linear_to_db));
	state.set("db_to_linear", wrap_function(&UtilityFunctions::db_to_linear));
	state.set("wrap", wrap_function(&UtilityFunctions::wrap));
	state.set("wrapi", wrap_function(&UtilityFunctions::wrapi));
	state.set("wrapf", wrap_function(&UtilityFunctions::wrapf));
	state.set("max", wrap_function(&UtilityFunctions::max_internal));
	state.set("maxi", wrap_function(&UtilityFunctions::maxi));
	state.set("maxf", wrap_function(&UtilityFunctions::maxf));
	state.set("min", wrap_function(&UtilityFunctions::min_internal));
	state.set("mini", wrap_function(&UtilityFunctions::mini));
	state.set("minf", wrap_function(&UtilityFunctions::minf));
	state.set("clamp", wrap_function(&UtilityFunctions::clamp));
	state.set("clampi", wrap_function(&UtilityFunctions::clampi));
	state.set("clampf", wrap_function(&UtilityFunctions::clampf));
	state.set("nearest_po2", wrap_function(&UtilityFunctions::nearest_po2));
	state.set("pingpong", wrap_function(&UtilityFunctions::pingpong));
	state.set("randomize", wrap_function(&UtilityFunctions::randomize));
	state.set("randi", wrap_function(&UtilityFunctions::randi));
	state.set("randf", wrap_function(&UtilityFunctions::randf));
	state.set("randi_range", wrap_function(&UtilityFunctions::randi_range));
	state.set("randf_range", wrap_function(&UtilityFunctions::randf_range));
	state.set("randfn", wrap_function(&UtilityFunctions::randfn));
	state.set("seed", wrap_function(&UtilityFunctions::seed));
	state.set("rand_from_seed", wrap_function(&UtilityFunctions::rand_from_seed));
	state.set("weakref", wrap_function(&UtilityFunctions::weakref));
	state.set("type_of", wrap_function(&UtilityFunctions::type_of));
	state.set("str", wrap_function(&UtilityFunctions::str_internal));
	state.set("error_string", wrap_function(&UtilityFunctions::error_string));
	state.set("print", wrap_function(&UtilityFunctions::print_internal));
	state.set("print_rich", wrap_function(&UtilityFunctions::print_rich_internal));
	state.set("printerr", wrap_function(&UtilityFunctions::printerr_internal));
	state.set("printt", wrap_function(&UtilityFunctions::printt_internal));
	state.set("prints", wrap_function(&UtilityFunctions::prints_internal));
	state.set("printraw", wrap_function(&UtilityFunctions::printraw_internal));
	state.set("print_verbose", wrap_function(&UtilityFunctions::print_verbose_internal));
	state.set("push_error", wrap_function(&UtilityFunctions::push_error_internal));
	state.set("push_warning", wrap_function(&UtilityFunctions::push_warning_internal));
	state.set("var_to_str", wrap_function(&UtilityFunctions::var_to_str));
	state.set("str_to_var", wrap_function(&UtilityFunctions::str_to_var));
	state.set("var_to_bytes", wrap_function(&UtilityFunctions::var_to_bytes));
	state.set("bytes_to_var", wrap_function(&UtilityFunctions::bytes_to_var));
	state.set("var_to_bytes_with_objects", wrap_function(&UtilityFunctions::var_to_bytes_with_objects));
	state.set("bytes_to_var_with_objects", wrap_function(&UtilityFunctions::bytes_to_var_with_objects));
	state.set("hash", wrap_function(&UtilityFunctions::hash));
	state.set("instance_from_id", wrap_function(&UtilityFunctions::instance_from_id));
	state.set("is_instance_id_valid", wrap_function(&UtilityFunctions::is_instance_id_valid));
	state.set("is_instance_valid", wrap_function(&UtilityFunctions::is_instance_valid));
	state.set("rid_allocate_id", wrap_function(&UtilityFunctions::rid_allocate_id));
	state.set("rid_from_int64", wrap_function(&UtilityFunctions::rid_from_int64));
	state.set("is_same", wrap_function(&UtilityFunctions::is_same));

	return 0;
}

