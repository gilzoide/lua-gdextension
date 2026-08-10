#pragma once

#include "../LuaFunction.hpp"

namespace luagdextension {

using namespace godot;

class LuaCallable : public CallableCustom {
	Ref<LuaFunction> _lua_func;
public:
	explicit LuaCallable(sol::protected_function func);
	~LuaCallable() = default;

	bool is_valid() const override;
	String get_as_text() const override;
	ObjectID get_object() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	uint32_t hash() const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const override;
	static Variant construct(sol::function func);

private:
	static bool compare_equal_func(const CallableCustom* p_a, const CallableCustom* p_b);
	static bool compare_less_func(const CallableCustom* p_a, const CallableCustom* p_b);
};

}
