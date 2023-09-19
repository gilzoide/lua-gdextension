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
#ifndef __UTILS_CONVERT_GODOT_LUA_HPP__
#define __UTILS_CONVERT_GODOT_LUA_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

Variant to_variant(const sol::object& object);
Variant to_variant(const sol::stack_object& object);
Variant to_variant(const sol::stack_proxy_base& object);
Variant to_variant(const sol::protected_function_result& function_result);
sol::stack_object to_lua(lua_State *lua_state, const Variant& value);

Array to_array(const sol::table& table);
Dictionary to_dictionary(const sol::table& table);

Variant do_string(sol::state_view& lua_state, const String& chunk, const String& chunkname = "");
Variant do_file(sol::state_view& lua_state, const String& filename, int buffer_size = 1024);

}

#endif  // __UTILS_CONVERT_GODOT_STD_HPP__
