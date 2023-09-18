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

#include "luaopen_godot.hpp"

#include "variant/register_types.hpp"

#include <sol/sol.hpp>

using namespace godot;
using namespace luagdextension;

int luaopen_godot(lua_State *L) {
	luaL_requiref(L, "godot.math", &luaopen_godot_math, 0);
	lua_pop(L, 1);

	return 0;
}


int luaopen_godot_math(lua_State *L) {
	sol::state_view state = L;

	register_vector2(state);
	register_vector2i(state);
	register_vector3(state);
	register_vector3i(state);
	register_vector4(state);
	register_vector4i(state);
	register_rect2(state);
	register_rect2i(state);
	register_plane(state);
	register_quaternion(state);
	register_aabb(state);
	register_basis(state);

	return 0;
}

