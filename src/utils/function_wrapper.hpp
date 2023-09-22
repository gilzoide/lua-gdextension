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
#ifndef __UTILS_FUNCTION_WRAPPER_HPP__
#define __UTILS_FUNCTION_WRAPPER_HPP__

#include <type_traits>

#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"

using namespace godot;

namespace luagdextension {

template<typename T>
struct WrappedArg {
	// T == Variant ? sol::object : T
	using type = typename std::conditional<std::is_same<std::decay_t<T>, Variant>::value, sol::object, T>::type;
};

template<typename T> decltype(auto) unwrap_variant(T value) { return value; }
template<> inline decltype(auto) unwrap_variant(sol::object value) { return to_variant(value); }

template<typename RetType, typename... VariantArgs>
decltype(auto) wrap_function(RetType(*f)(VariantArgs... args)) {
	return [f](typename WrappedArg<VariantArgs>::type... lua_args) {
		return f(unwrap_variant(lua_args)...);
	};
}

template<typename T>
decltype(auto) wrap_function(T(*f)(const Variant **, GDExtensionInt)) {
	return [f](sol::variadic_args args) {
		VariantArguments var_args = args;
		return f(var_args.argv(), var_args.argc());
	};
}

}

#endif  // __UTILS_FUNCTION_WRAPPER_HPP__
