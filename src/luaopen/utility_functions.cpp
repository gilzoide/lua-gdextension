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
#include "../generated/utility_functions.hpp"
#include "../utils/VariantArguments.hpp"
#include "../utils/function_wrapper.hpp"
#include "../utils/string_literal.hpp"

#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace luagdextension;

template<typename RetType, StringLiteral func_name, size_t func_hash> sol::object _call_variadic_utility_function(sol::variadic_args lua_args) {
	static GDExtensionPtrUtilityFunction _gde_function = internal::gdextension_interface_variant_get_ptr_utility_function(StringName(func_name)._native_ptr(), func_hash);
	CHECK_METHOD_BIND_RET(_gde_function, sol::nil);
	VariantArguments args(lua_args);
	if constexpr (std::is_void_v<RetType>) {
		_gde_function(nullptr, reinterpret_cast<GDExtensionConstVariantPtr *>(args.argv()), args.argc());
		return sol::nil;
	}
	else {
		RetType ret;
		_gde_function(&ret, reinterpret_cast<GDExtensionConstVariantPtr *>(args.argv()), args.argc());
		return to_lua(lua_args.lua_state(), ret);
	}
}

extern "C" int luaopen_godot_utility_functions(lua_State *L) {
	sol::state_view state = L;

	register_utility_functions(state);

	// In Lua, `print` separates passed values with "\t", so we bind it to Godot's `printt`
	state.do_string("print = printt");

	return 0;
}

