#pragma once

#include "../LuaFunction.hpp"

namespace luagdextension {

using namespace godot;

class LuaCallable : public CallableCustom {
	Ref<LuaFunction> _lua_func;
public:
	explicit LuaCallable(sol::function func) : _lua_func{memnew(LuaFunction(func))} {};
	virtual ~LuaCallable();

	bool is_valid() const override;
	String get_as_text() const override;
	ObjectID get_object() const override;
	virtual CompareEqualFunc get_compare_equal_func() const override;
	virtual CompareLessFunc get_compare_less_func() const override;
	uint32_t hash() const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const override;
	static void register_lua(lua_State *L);
	int get_argument_count(bool &r_is_valid) const override;
	static Variant construct(sol::function func);
};

}
