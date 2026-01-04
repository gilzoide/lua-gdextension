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
#ifndef __UTILS_METHOD_BIND_IMPL_HPP__
#define __UTILS_METHOD_BIND_IMPL_HPP__

#include "Class.hpp"
#include "VariantType.hpp"
#include "../script-language/LuaScriptInstance.hpp"

namespace luagdextension {

class BaseMethodBind {
public:
	BaseMethodBind(const StringName& method_name);
	virtual ~BaseMethodBind() = default;

	const StringName& get_method_name() const;
	virtual sol::object call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const = 0;

protected:
	StringName method_name;

	template<typename Self>
	static void register_subtype(sol::state_view& state, std::string_view name) {
		state.new_usertype<Self>(
			name,
			sol::no_constructor,
			sol::meta_function::call, &Self::call,
			sol::meta_function::to_string, &BaseMethodBind::get_method_name
		);
	}
};


class ClassMethodBind : public BaseMethodBind {
public:
	ClassMethodBind(const Class& cls, const StringName& method_name);

	sol::object call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const override;
	static void register_usertype(sol::state_view& state);

private:
	Class cls;
};


class LuaScriptInstanceMethodBind : public BaseMethodBind {
public:
	LuaScriptInstanceMethodBind(LuaScriptInstance *instance, const StringName& method_name);

	Callable to_callable() const;
	sol::object call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const override;
	static void register_usertype(sol::state_view& state);

protected:
	Object *instance_owner;
};


class VariantMethodBind : public BaseMethodBind {
public:
	VariantMethodBind(const Variant& variant, const StringName& method_name);

	Callable to_callable() const;
	sol::object call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const override;
	static void register_usertype(sol::state_view& state);

protected:
	Variant variant;
};


class VariantTypeMethodBind : public BaseMethodBind {
public:
	VariantTypeMethodBind(const VariantType& type, const StringName& method_name);

	sol::object call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const override;
	static void register_usertype(sol::state_view& state);

protected:
	VariantType type;
};

}

#endif  // __UTILS_METHOD_BIND_IMPL_HPP__
