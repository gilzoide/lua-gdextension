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
#ifndef __LUA_FUNCTION_HPP__
#define __LUA_FUNCTION_HPP__

#include <array>

#include "LuaError.hpp"
#include "LuaObject.hpp"

using namespace godot;

namespace luagdextension {

class LuaScriptInstance;

class LuaFunction : public LuaObjectSubclass<sol::protected_function> {
	GDCLASS(LuaFunction, LuaObject);

public:
	LuaFunction();
	LuaFunction(sol::protected_function&& function);
	LuaFunction(const sol::protected_function& function);

	Variant invokev(const Array& args);
	Variant invoke(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
	Variant invoke_method(const Variant& self, const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
	Variant invoke_method(LuaScriptInstance *self, const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);

	template<typename... Args>
	Variant try_call(Args&&... args) {
		std::array<Variant, sizeof...(Args)> vars { Variant(std::forward<Args>(args))... };
		std::array<const Variant *, sizeof...(Args)> var_ptrs;
		for (int i = 0; i < vars.size(); i++) {
			var_ptrs[i] = &vars[i];
		}
		GDExtensionCallError err;
		Variant result = invoke((const Variant **) var_ptrs.data(), (GDExtensionInt) vars.size(), err);
		if (LuaError *error = Object::cast_to<LuaError>(result)) {
			ERR_PRINT(error->get_message());
			return Variant();
		}
		else {
			return result;
		}
	}
	
	template<typename... Args>
	Variant try_call_method(LuaScriptInstance *self, Args&&... args) {
		std::array<Variant, sizeof...(Args)> vars { Variant(std::forward<Args>(args))... };
		std::array<const Variant *, sizeof...(Args)> var_ptrs;
		for (int i = 0; i < vars.size(); i++) {
			var_ptrs[i] = &vars[i];
		}
		GDExtensionCallError err;
		Variant result = invoke_method(self, (const Variant **) var_ptrs.data(), (GDExtensionInt) vars.size(), err);
		if (LuaError *error = Object::cast_to<LuaError>(result)) {
			ERR_PRINT(error->get_message());
			return Variant();
		}
		else {
			return result;
		}
	}

	Callable to_callable() const;

	const sol::protected_function& get_function() const;

protected:
	static void _bind_methods();
};

}

#endif  // __LUA_FUNCTION_HPP__
