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
#ifndef __UTILS_CONVERT_GODOT_LUA_HPP__
#define __UTILS_CONVERT_GODOT_LUA_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/variant/variant.hpp>

using namespace godot;

namespace luagdextension {

class LuaTable;

Variant to_variant(const sol::object& object);
Variant to_variant(const sol::stack_object& object);
Variant to_variant(const sol::stack_proxy_base& object);
Variant to_variant(const sol::protected_function_result& function_result, bool return_lua_error);
Variant to_variant(const sol::load_result& load_result);
Variant to_variant(lua_State *L, int index);
sol::stack_object lua_push(lua_State *L, const Variant& value);
sol::object to_lua(lua_State *L, const Variant& value);

void fill_array(Array& array, const sol::table& table);
void fill_array(Array& array, const sol::variadic_args& args);
void fill_dictionary(Dictionary& dict, const sol::table& table);

Array to_array(const sol::table& table);
Dictionary to_dictionary(const sol::table& table);
sol::table to_table(sol::state_view& state, const Dictionary& dictionary);

void lua_push_function(lua_State *L, const Callable& callable);
sol::protected_function to_lua_function(lua_State *L, const Callable& callable);
Variant callable_call(const Callable& callable, const sol::variadic_args& args);

sol::object variant_static_call_string_name(sol::this_state state, Variant::Type type, const StringName& method, const sol::variadic_args& args);
sol::object variant_call_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args);
sol::object variant_call(sol::this_state state, Variant& variant, const char *method, const sol::variadic_args& args);
std::tuple<bool, sol::object> variant_pcall_string_name(sol::this_state state, Variant& variant, const StringName& method, const sol::variadic_args& args);
std::tuple<bool, sol::object> variant_pcall(sol::this_state state, Variant& variant, const char *method, const sol::variadic_args& args);

Variant do_buffer(sol::state_view& lua_state, const PackedByteArray& chunk, const String& chunkname = "", sol::load_mode mode = sol::load_mode::any, LuaTable *env = nullptr);
Variant do_file(sol::state_view& lua_state, const String& filename, sol::load_mode mode = sol::load_mode::any, LuaTable *env = nullptr);
Variant load_buffer(sol::state_view& lua_state, const PackedByteArray& chunk, const String& chunkname = "", sol::load_mode mode = sol::load_mode::any, LuaTable *env = nullptr);
Variant load_file(sol::state_view& lua_state, const String& filename, sol::load_mode mode = sol::load_mode::any, LuaTable *env = nullptr);

void lua_error(lua_State *L, const GDExtensionCallError& call_error, const String& prefix_message);

}

#endif  // __UTILS_CONVERT_GODOT_LUA_HPP__
