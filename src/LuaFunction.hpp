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
#ifndef __LUA_FUNCTION_HPP__
#define __LUA_FUNCTION_HPP__

#include "LuaObject.hpp"

using namespace godot;

namespace luagdextension {

class LuaDebug;
class VariantArguments;

class LuaFunction : public LuaObjectSubclass<sol::protected_function> {
	GDCLASS(LuaFunction, LuaObject);

public:
	LuaFunction();
	LuaFunction(sol::protected_function&& function);
	LuaFunction(const sol::protected_function& function);

	Variant invokev(const Array& args);
	Variant invoke(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);

	static Variant invoke_lua(Ref<LuaFunction> f, const VariantArguments& args, bool return_lua_error);
	static Variant invoke_lua(const sol::protected_function& f, const VariantArguments& args, bool return_lua_error);

	Callable to_callable() const;
	Ref<LuaDebug> get_debug_info() const;

	const sol::protected_function& get_function() const;

protected:
	static void _bind_methods();
};

}

#endif  // __LUA_FUNCTION_HPP__
