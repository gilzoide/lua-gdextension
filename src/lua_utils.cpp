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
#include <string_view>

#include "LuaError.hpp"
#include "lua_utils.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>

using namespace godot;

namespace luagdextension {

void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	if (nsize == 0) {
		if (ptr != nullptr) {
			memfree(ptr);
		}
		return nullptr;
	}
	else {
		return memrealloc(ptr, nsize);
	}
}

Variant to_variant(const sol::stack_proxy_base& stack) {
	switch (stack.get_type()) {
		case sol::type::boolean:
			return stack.get<bool>();

		case sol::type::string: {
			auto sv = stack.get<std::string_view>();
			return String::utf8(sv.data(), sv.length());
		}

		case sol::type::number:
#if LUA_VERSION_NUM >= 503
			if (lua_isinteger(stack.lua_state(), stack.stack_index())) {
				return stack.get<int64_t>();
			}
#endif
			return stack.get<double>();

		case sol::type::thread:
		case sol::type::function:
		case sol::type::userdata:
		case sol::type::lightuserdata:
		case sol::type::table:
			WARN_PRINT_ONCE_ED("Lua type not yet supported");

		default:
		case sol::type::none:
		case sol::type::lua_nil:
			return Variant();
	}
}

Variant to_variant(const sol::protected_function_result& function_result) {
	if (!function_result.valid()) {
		LuaError *error = memnew(LuaError);
		error->set_status((LuaError::Status) function_result.status());
		error->set_message(((sol::error) function_result).what());
		return error;
	}

	switch (function_result.return_count()) {
		case 0:
			return Variant();

		case 1:
			return to_variant(function_result[0]);

		default:
			auto arr = Array();
			for (auto it = function_result.cbegin(); it != function_result.cend(); it++) {
				arr.append(to_variant(*it));
			}
			return arr;
	}
}

}
