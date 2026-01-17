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

/**
 * Include this file instead of <sol/sol.hpp> to have custom type conversions available.
 *
 * Implemented custom types: String <-> Lua string, StringName <-> Lua string
 */
#ifndef __CUSTOM_SOL_HPP__
#define __CUSTOM_SOL_HPP__

#include <godot_cpp/variant/variant.hpp>
#include <sol/sol.hpp>

namespace luagdextension {
	class VariantArguments;
}

using namespace godot;

/// Custom String <-> Lua string conversions
template<typename Handler>
bool sol_lua_check(sol::types<String>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking) {
	bool success = sol::stack::check<const char *>(L, index, handler);
	tracking.use(1);
	return success;
}
String sol_lua_get(sol::types<String>, lua_State* L, int index, sol::stack::record& tracking);
int sol_lua_push(lua_State* L, const String& str);

/// Custom StringName <-> Lua string conversions
template<typename Handler>
bool sol_lua_check(sol::types<StringName>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking) {
	bool success = sol::stack::check<const char *>(L, index, handler);
	tracking.use(1);
	return success;
}
StringName sol_lua_get(sol::types<StringName>, lua_State* L, int index, sol::stack::record& tracking);
int sol_lua_push(lua_State* L, const StringName& str);

// Custom push conversions to Variant
int sol_lua_push(lua_State* L, const Vector2 &v);
int sol_lua_push(lua_State* L, const Vector2i &v);
int sol_lua_push(lua_State* L, const Rect2 &v);
int sol_lua_push(lua_State* L, const Rect2i &v);
int sol_lua_push(lua_State* L, const Vector3 &v);
int sol_lua_push(lua_State* L, const Vector3i &v);
int sol_lua_push(lua_State* L, const Transform2D &v);
int sol_lua_push(lua_State* L, const Vector4 &v);
int sol_lua_push(lua_State* L, const Vector4i &v);
int sol_lua_push(lua_State* L, const Plane &v);
int sol_lua_push(lua_State* L, const Quaternion &v);
int sol_lua_push(lua_State* L, const AABB &v);
int sol_lua_push(lua_State* L, const Basis &v);
int sol_lua_push(lua_State* L, const Transform3D &v);
int sol_lua_push(lua_State* L, const Projection &v);
int sol_lua_push(lua_State* L, const Color &v);
int sol_lua_push(lua_State* L, const NodePath &v);
int sol_lua_push(lua_State* L, const RID &v);
int sol_lua_push(lua_State* L, const ObjectID &v);
int sol_lua_push(lua_State* L, Object *v);
int sol_lua_push(lua_State* L, const Callable &v);
int sol_lua_push(lua_State* L, const Signal &v);
int sol_lua_push(lua_State* L, const Dictionary &v);
int sol_lua_push(lua_State* L, const Array &v);
int sol_lua_push(lua_State* L, const PackedByteArray &v);
int sol_lua_push(lua_State* L, const PackedInt32Array &v);
int sol_lua_push(lua_State* L, const PackedInt64Array &v);
int sol_lua_push(lua_State* L, const PackedFloat32Array &v);
int sol_lua_push(lua_State* L, const PackedFloat64Array &v);
int sol_lua_push(lua_State* L, const PackedStringArray &v);
int sol_lua_push(lua_State* L, const PackedVector2Array &v);
int sol_lua_push(lua_State* L, const PackedVector3Array &v);
int sol_lua_push(lua_State* L, const PackedColorArray &v);
int sol_lua_push(lua_State* L, const PackedVector4Array &v);

// Custom push variadic argumens
int sol_lua_push(lua_State* L, const luagdextension::VariantArguments &v);

// A version of lua_resume that works with any version of Lua
int resume_lua_coroutine(lua_State *L, int nargs, int *nresults);

#if LUA_VERSION_NUM >= 504
#define HAVE_LUA_WARN
#endif

// Polyfill for some Lua 5.2 stuff
#ifndef LUA_PATH_SEP
#define LUA_PATH_SEP ";"
#endif
#ifndef LUA_EXEC_DIR
#define LUA_EXEC_DIR "!"
#endif

template<typename... Args>
void lua_push_closure(lua_State *L, lua_CFunction f, Args&&... args) {
	sol::stack::push(L, std::forward<Args>(args)...);
	lua_pushcclosure(L, f, sizeof...(Args));
}

template<typename... Args>
sol::protected_function to_lua_closure(lua_State *L, lua_CFunction f, Args&&... args) {
	lua_push_closure(L, f, std::forward<Args>(args)...);
	sol::protected_function result(L, -1);
	lua_pop(L, 1);
	return result;
}

#endif
