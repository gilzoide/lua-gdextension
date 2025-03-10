/**
 * Copyright (C) 2025 Gil Barbosa Reis.
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
#ifndef __UTILS_FUNCTION_WRAPPER_HPP__
#define __UTILS_FUNCTION_WRAPPER_HPP__

#include <type_traits>

#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"

using namespace godot;

namespace luagdextension::function_wrapper {

/**
 * Argument wrapper for converting Lua objects to Variant.
 * If T is Variant, wrapper argument will be sol::object.
 * Otherwise, use T.
 */
template<typename T>
struct WrappedArg {
	// T == Variant ? sol::object : T
	using type = typename std::conditional<std::is_same<std::decay_t<T>, Variant>::value, sol::stack_object, T>::type;
};

/// Wrap an argument from Lua to Variant.
template<typename T> decltype(auto) wrap_argument(T value) { return value; }
template<> inline decltype(auto) wrap_argument(sol::stack_object value) { return to_variant(value); }

/// Unwrap Variant values to Lua objects.
template<typename T> decltype(auto) unwrap_return(T value, sol::this_state state) { return value; }
template<> inline decltype(auto) unwrap_return(Variant value, sol::this_state state) { return to_lua(state, value); }

}

namespace luagdextension {

/**
 * Wrap a function to be used in Lua.
 * Variant arguments are converted nicely, so the runtime doesn't crash.
 */
template<typename RetType, typename... VariantArgs>
decltype(auto) wrap_function(RetType(*f)(VariantArgs... args)) {
	return [f](typename function_wrapper::WrappedArg<VariantArgs>::type... lua_args, sol::this_state state) {
		RetType result = f(function_wrapper::wrap_argument(lua_args)...);
		return function_wrapper::unwrap_return(result, state);
	};
}
template<typename... VariantArgs>
inline decltype(auto) wrap_function(void(*f)(VariantArgs... args)) {
	return [f](typename function_wrapper::WrappedArg<VariantArgs>::type... lua_args, sol::this_state state) {
		f(function_wrapper::wrap_argument(lua_args)...);
	};
}

/**
 * Wrap a variadic function to be used in Lua.
 * Variant arguments are converted nicely, so the runtime won't crash.
 */
template<typename RetType>
decltype(auto) wrap_variadic_function(RetType(*f)(const Variant **, GDExtensionInt)) {
	return [f](sol::variadic_args args, sol::this_state state) {
		VariantArguments var_args = args;
		RetType result = f(var_args.argv(), var_args.argc());
		return function_wrapper::unwrap_return(result, state);
	};
}
template<>
inline decltype(auto) wrap_variadic_function(void(*f)(const Variant **, GDExtensionInt)) {
	return [f](sol::variadic_args args, sol::this_state state) {
		VariantArguments var_args = args;
		f(var_args.argv(), var_args.argc());
	};
}

}

#endif  // __UTILS_FUNCTION_WRAPPER_HPP__
