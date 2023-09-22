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

#include <godot_cpp/variant/utility_functions.hpp>

#include "../utils/custom_sol.hpp"

using namespace godot;

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
	state.set("floor", &UtilityFunctions::floor);
	state.set("floorf", &UtilityFunctions::floorf);
	state.set("floori", &UtilityFunctions::floori);
	state.set("ceil", &UtilityFunctions::ceil);
	state.set("ceilf", &UtilityFunctions::ceilf);
	state.set("ceili", &UtilityFunctions::ceili);
	state.set("round", &UtilityFunctions::round);
	state.set("roundf", &UtilityFunctions::roundf);
	state.set("roundi", &UtilityFunctions::roundi);
	state.set("abs", &UtilityFunctions::abs);
	state.set("absf", &UtilityFunctions::absf);
	state.set("absi", &UtilityFunctions::absi);
	state.set("sign", &UtilityFunctions::sign);
	state.set("signf", &UtilityFunctions::signf);
	state.set("signi", &UtilityFunctions::signi);
	state.set("snapped", &UtilityFunctions::snapped);
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
	state.set("lerp", &UtilityFunctions::lerp);
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
	state.set("wrap", &UtilityFunctions::wrap);
	state.set("wrapi", &UtilityFunctions::wrapi);
	state.set("wrapf", &UtilityFunctions::wrapf);
	/* private: static Variant max_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static Variant max(const Variant &arg1, const Variant &arg2, const Args&... args) { */
	/* 	std::array<Variant, 2 + sizeof...(Args)> variant_args { arg1, arg2, Variant(args)... }; */
	/* 	std::array<const Variant *, 2 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	return max_internal(call_args.data(), variant_args.size()); */
	/* } */
	state.set("maxi", &UtilityFunctions::maxi);
	state.set("maxf", &UtilityFunctions::maxf);
	/* private: static Variant min_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static Variant min(const Variant &arg1, const Variant &arg2, const Args&... args) { */
	/* 	std::array<Variant, 2 + sizeof...(Args)> variant_args { arg1, arg2, Variant(args)... }; */
	/* 	std::array<const Variant *, 2 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	return min_internal(call_args.data(), variant_args.size()); */
	/* } */
	state.set("mini", &UtilityFunctions::mini);
	state.set("minf", &UtilityFunctions::minf);
	state.set("clamp", &UtilityFunctions::clamp);
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
	state.set("weakref", &UtilityFunctions::weakref);
	state.set("type_of", &UtilityFunctions::type_of);
	/* private: static String str_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static String str(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	return str_internal(call_args.data(), variant_args.size()); */
	/* } */
	state.set("error_string", &UtilityFunctions::error_string);
	/* private: static void print_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void print(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	print_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void print_rich_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void print_rich(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	print_rich_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void printerr_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void printerr(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	printerr_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void printt_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void printt(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	printt_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void prints_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void prints(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	prints_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void printraw_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void printraw(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	printraw_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void print_verbose_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void print_verbose(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	print_verbose_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void push_error_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void push_error(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	push_error_internal(call_args.data(), variant_args.size()); */
	/* } */
	/* private: static void push_warning_internal(const Variant **args, GDExtensionInt arg_count); */
	/* public: template<class... Args> static void push_warning(const Variant &arg1, const Args&... args) { */
	/* 	std::array<Variant, 1 + sizeof...(Args)> variant_args { arg1, Variant(args)... }; */
	/* 	std::array<const Variant *, 1 + sizeof...(Args)> call_args; */
	/* 	for(size_t i = 0; i < variant_args.size(); i++) { */
	/* 		call_args[i] = &variant_args[i]; */
	/* 	} */
	/* 	push_warning_internal(call_args.data(), variant_args.size()); */
	/* } */
	state.set("var_to_str", &UtilityFunctions::var_to_str);
	state.set("str_to_var", &UtilityFunctions::str_to_var);
	state.set("var_to_bytes", &UtilityFunctions::var_to_bytes);
	state.set("bytes_to_var", &UtilityFunctions::bytes_to_var);
	state.set("var_to_bytes_with_objects", &UtilityFunctions::var_to_bytes_with_objects);
	state.set("bytes_to_var_with_objects", &UtilityFunctions::bytes_to_var_with_objects);
	state.set("hash", &UtilityFunctions::hash);
	state.set("instance_from_id", &UtilityFunctions::instance_from_id);
	state.set("is_instance_id_valid", &UtilityFunctions::is_instance_id_valid);
	state.set("is_instance_valid", &UtilityFunctions::is_instance_valid);
	state.set("rid_allocate_id", &UtilityFunctions::rid_allocate_id);
	state.set("rid_from_int64", &UtilityFunctions::rid_from_int64);
	state.set("is_same", &UtilityFunctions::is_same);

	return 0;
}


