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

	state.set("sin", &UtilityFunctions::sin);
	state.set("cos", &UtilityFunctions::cos);
	state.set("tan", &UtilityFunctions::tan);
	state.set("sinh", &UtilityFunctions::sinh);
	state.set("cosh", &UtilityFunctions::cosh);
	state.set("tanh", &UtilityFunctions::tanh);
	state.set("asin", &UtilityFunctions::asin);
	state.set("acos", &UtilityFunctions::acos);
	state.set("atan", &UtilityFunctions::atan);
	state.set("atan2", &UtilityFunctions::atan2);
	state.set("sqrt", &UtilityFunctions::sqrt);
	state.set("fmod", &UtilityFunctions::fmod);
	state.set("fposmod", &UtilityFunctions::fposmod);
	state.set("posmod", &UtilityFunctions::posmod);
	state.set("floor", wrap_function(&UtilityFunctions::floor));
	state.set("floorf", &UtilityFunctions::floorf);
	state.set("floori", &UtilityFunctions::floori);
	state.set("ceil", wrap_function(&UtilityFunctions::ceil));
	state.set("ceilf", &UtilityFunctions::ceilf);
	state.set("ceili", &UtilityFunctions::ceili);
	state.set("round", wrap_function(&UtilityFunctions::round));
	state.set("roundf", &UtilityFunctions::roundf);
	state.set("roundi", &UtilityFunctions::roundi);
	state.set("abs", wrap_function(&UtilityFunctions::abs));
	state.set("absf", &UtilityFunctions::absf);
	state.set("absi", &UtilityFunctions::absi);
	state.set("sign", wrap_function(&UtilityFunctions::sign));
	state.set("signf", &UtilityFunctions::signf);
	state.set("signi", &UtilityFunctions::signi);
	state.set("snapped", wrap_function(&UtilityFunctions::snapped));
	state.set("snappedf", &UtilityFunctions::snappedf);
	state.set("snappedi", &UtilityFunctions::snappedi);
	state.set("pow", &UtilityFunctions::pow);
	state.set("log", &UtilityFunctions::log);
	state.set("exp", &UtilityFunctions::exp);
	state.set("is_nan", &UtilityFunctions::is_nan);
	state.set("is_inf", &UtilityFunctions::is_inf);
	state.set("is_equal_approx", &UtilityFunctions::is_equal_approx);
	state.set("is_zero_approx", &UtilityFunctions::is_zero_approx);
	state.set("is_finite", &UtilityFunctions::is_finite);
	state.set("ease", &UtilityFunctions::ease);
	state.set("step_decimals", &UtilityFunctions::step_decimals);
	state.set("lerp", wrap_function(&UtilityFunctions::lerp));
	state.set("lerpf", &UtilityFunctions::lerpf);
	state.set("cubic_interpolate", &UtilityFunctions::cubic_interpolate);
	state.set("cubic_interpolate_angle", &UtilityFunctions::cubic_interpolate_angle);
	state.set("cubic_interpolate_in_time", &UtilityFunctions::cubic_interpolate_in_time);
	state.set("cubic_interpolate_angle_in_time", &UtilityFunctions::cubic_interpolate_angle_in_time);
	state.set("bezier_interpolate", &UtilityFunctions::bezier_interpolate);
	state.set("bezier_derivative", &UtilityFunctions::bezier_derivative);
	state.set("lerp_angle", &UtilityFunctions::lerp_angle);
	state.set("inverse_lerp", &UtilityFunctions::inverse_lerp);
	state.set("remap", &UtilityFunctions::remap);
	state.set("smoothstep", &UtilityFunctions::smoothstep);
	state.set("move_toward", &UtilityFunctions::move_toward);
	state.set("deg_to_rad", &UtilityFunctions::deg_to_rad);
	state.set("rad_to_deg", &UtilityFunctions::rad_to_deg);
	state.set("linear_to_db", &UtilityFunctions::linear_to_db);
	state.set("db_to_linear", &UtilityFunctions::db_to_linear);
	state.set("wrap", wrap_function(&UtilityFunctions::wrap));
	state.set("wrapi", &UtilityFunctions::wrapi);
	state.set("wrapf", &UtilityFunctions::wrapf);
	state.set("max", wrap_function(&UtilityFunctions::max_internal));
	state.set("maxi", &UtilityFunctions::maxi);
	state.set("maxf", &UtilityFunctions::maxf);
	state.set("min", wrap_function(&UtilityFunctions::min_internal));
	state.set("mini", &UtilityFunctions::mini);
	state.set("minf", &UtilityFunctions::minf);
	state.set("clamp", wrap_function(&UtilityFunctions::clamp));
	state.set("clampi", &UtilityFunctions::clampi);
	state.set("clampf", &UtilityFunctions::clampf);
	state.set("nearest_po2", &UtilityFunctions::nearest_po2);
	state.set("pingpong", &UtilityFunctions::pingpong);
	state.set("randomize", &UtilityFunctions::randomize);
	state.set("randi", &UtilityFunctions::randi);
	state.set("randf", &UtilityFunctions::randf);
	state.set("randi_range", &UtilityFunctions::randi_range);
	state.set("randf_range", &UtilityFunctions::randf_range);
	state.set("randfn", &UtilityFunctions::randfn);
	state.set("seed", &UtilityFunctions::seed);
	state.set("rand_from_seed", &UtilityFunctions::rand_from_seed);
	state.set("weakref", wrap_function(&UtilityFunctions::weakref));
	state.set("type_of", wrap_function(&UtilityFunctions::type_of));
	state.set("str", wrap_function(&UtilityFunctions::str_internal));
	state.set("error_string", &UtilityFunctions::error_string);
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
	state.set("instance_from_id", &UtilityFunctions::instance_from_id);
	state.set("is_instance_id_valid", &UtilityFunctions::is_instance_id_valid);
	state.set("is_instance_valid", wrap_function(&UtilityFunctions::is_instance_valid));
	state.set("rid_allocate_id", &UtilityFunctions::rid_allocate_id);
	state.set("rid_from_int64", &UtilityFunctions::rid_from_int64);
	state.set("is_same", wrap_function(&UtilityFunctions::is_same));

	return 0;
}

