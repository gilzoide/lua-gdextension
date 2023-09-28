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
#include "LuaFunction.hpp"

#include "LuaState.hpp"

#include <godot_cpp/core/error_macros.hpp>

namespace luagdextension {

LuaFunction::LuaFunction() : function() {
	ERR_PRINT("LuaUserdata should never be instantiated manually!");
}

LuaFunction::LuaFunction(sol::protected_function&& function) : function(function) {}

LuaFunction::LuaFunction(const sol::protected_function& function) : function(function) {}

LuaFunction::~LuaFunction() {
	if (function.valid() && LuaState::is_valid(function.lua_state())) {
		function.~basic_protected_function();
	}
}

void LuaFunction::_bind_methods() {

}

String LuaFunction::_to_string() const {
	return String("[LuaFunction:0x%x]") % (int64_t) function.pointer();
}

}
