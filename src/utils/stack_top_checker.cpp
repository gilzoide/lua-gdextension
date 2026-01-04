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

#ifdef DEBUG_ENABLED

#include <godot_cpp/core/error_macros.hpp>

#include "custom_sol.hpp"
#include "stack_top_checker.hpp"

namespace luagdextension {

StackTopChecker::StackTopChecker(lua_State *L, int expected_extra_values)
	: L(L)
	, expected_extra_values(expected_extra_values)
	, previous_top(lua_gettop(L))
{
}

StackTopChecker::~StackTopChecker() {
	int top = lua_gettop(L);
	if (top != previous_top + expected_extra_values) {
		ERR_PRINT(String("Invalid stack top, expected %d, found %d") % Array::make(previous_top + expected_extra_values, top));
	}
}

}

#endif
