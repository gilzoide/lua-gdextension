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

#include "godot.hpp"

#include "../utils/module_names.hpp"

#include <sol/sol.hpp>

using namespace luagdextension;

extern "C" int luaopen_godot(lua_State *L) {
	sol::state_view state = L;

	state.require(module_names::variant, &luaopen_godot_variant, false);
	state.require(module_names::utility_functions, &luaopen_godot_utility_functions, false);
	state.require(module_names::classes, &luaopen_godot_classes, false);
	state.require(module_names::singleton_access, &luaopen_godot_singleton_access, false);
	state.require(module_names::enums, &luaopen_godot_enums, false);
	state.require(module_names::local_paths, &luaopen_godot_local_paths, false);

	return 0;
}

