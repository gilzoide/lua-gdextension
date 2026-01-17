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

#include "LuaScriptSignal.hpp"

#include "../utils/VariantArguments.hpp"
#include "../utils/custom_sol.hpp"

namespace luagdextension {

MethodInfo LuaScriptSignal::to_method_info() const {
	MethodInfo mi;
	mi.name = name;
	for (int i = 0; i < arguments.size(); i++) {
		mi.arguments.push_back(PropertyInfo(Variant::Type::NIL, arguments[i]));
	}
	return mi;
}

Dictionary LuaScriptSignal::to_dictionary() const {
	return to_method_info();
}

static LuaScriptSignal lua_signal(sol::variadic_args arguments) {
	LuaScriptSignal signal = {
		.arguments = VariantArguments(arguments).get_array(),
	};
	return signal;
}

void LuaScriptSignal::register_lua(lua_State *L) {
	sol::state_view state(L);
	state.new_usertype<LuaScriptSignal>(
		"LuaScriptSignal",
		sol::no_construction()
	);
	state.set("signal", &lua_signal);
}

}
