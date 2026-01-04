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
#ifndef __UTILS_METATABLE_HPP__
#define __UTILS_METATABLE_HPP__

#include <sol/sol.hpp>

namespace luagdextension {

template<typename ref_t>
sol::optional<sol::protected_function> get_metamethod(const sol::basic_table<ref_t>& table, sol::meta_function function) {
	sol::optional<sol::metatable> opt_metatable = table[sol::metatable_key];
	if (opt_metatable.has_value()) {
		return opt_metatable.value()[function];
	}
	else {
		return {};
	}
}

template<typename ref_t, typename... Args>
sol::optional<sol::protected_function_result> call_metamethod(const sol::basic_table<ref_t>& table, sol::meta_function function, Args... args) {
	sol::optional<sol::protected_function> opt_method = get_metamethod(table, function);
	if (opt_method.has_value()) {
		return opt_method.value().call(table, args...);
	}
	else {
		return {};
	}
}

}

#endif  // __UTILS_METATABLE_HPP__
