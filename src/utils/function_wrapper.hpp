/**
 * Copyright (C) 2026 Gil Barbosa Reis.
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
	using type = typename std::conditional_t<std::is_same_v<std::decay_t<T>, Variant>, sol::stack_object, T>;
};

/// Wrap an argument from Lua to Variant.
template<typename T> decltype(auto) wrap_argument(T value) { return value; }
template<> inline decltype(auto) wrap_argument(sol::stack_object value) { return to_variant(value); }

/// Unwrap Variant values to Lua objects.
template<typename T> decltype(auto) unwrap_return(T value, sol::this_state state) { return value; }
template<> inline decltype(auto) unwrap_return(Variant value, sol::this_state state) { return to_lua(state, value); }

template<typename RetType, typename... VariantArgs, size_t... I>
RetType call_wrapped_function(lua_State *L, RetType(*f)(VariantArgs...), std::index_sequence<I...>) {
	return f(wrap_argument(sol::stack::get<typename function_wrapper::WrappedArg<VariantArgs>::type>(L, I + 1))...);
}

template<typename... VariantArgs, size_t... I>
void call_wrapped_function(lua_State *L, void(*f)(VariantArgs...), std::index_sequence<I...>) {
	f(wrap_argument(sol::stack::get<typename function_wrapper::WrappedArg<VariantArgs>::type>(L, I + 1))...);
}

template<typename RetType, typename... VariantArgs>
int lua_call_wrapped_function(lua_State *L) {
	RetType(*f)(VariantArgs...) = (RetType(*)(VariantArgs...)) lua_touserdata(L, lua_upvalueindex(1));
	RetType result = call_wrapped_function(L, f, std::index_sequence_for<VariantArgs...>());
	lua_push(L, result);
	return 1;
}

template<typename... VariantArgs>
int lua_call_wrapped_void_function(lua_State *L) {
	void(*f)(VariantArgs...) = (void(*)(VariantArgs...)) lua_touserdata(L, lua_upvalueindex(1));
	call_wrapped_function(L, f, std::index_sequence_for<VariantArgs...>());
	return 0;
}

}

namespace luagdextension {

/**
 * Wrap a function to be used in Lua.
 * Variant arguments are converted nicely, so the runtime doesn't crash.
 */
template<typename RetType, typename... VariantArgs>
decltype(auto) wrap_function(lua_State *L, RetType(*f)(VariantArgs... args)) {
	return to_lua_closure(L, function_wrapper::lua_call_wrapped_function<RetType, VariantArgs...>, (void *) f);
}
template<typename... VariantArgs>
inline decltype(auto) wrap_function(lua_State *L, void(*f)(VariantArgs... args)) {
	return to_lua_closure(L, function_wrapper::lua_call_wrapped_void_function<VariantArgs...>, (void *) f);
}

}

#endif  // __UTILS_FUNCTION_WRAPPER_HPP__
