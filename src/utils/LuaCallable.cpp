#include "LuaCallable.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"

namespace luagdextension {

LuaCallable::LuaCallable(sol::protected_function func)
	: _lua_func(LuaObject::wrap_object<LuaFunction>(func))
{
}

LuaCallable::CompareEqualFunc LuaCallable::get_compare_equal_func() const {
	return &compare_equal_func;
}

LuaCallable::CompareLessFunc LuaCallable::get_compare_less_func() const {
	return &compare_less_func;
}

bool LuaCallable::is_valid() const {
	return _lua_func->get_function().valid();
}

ObjectID LuaCallable::get_object() const {
	if (_lua_func.is_valid()) {
		return ObjectID(_lua_func->get_instance_id());
	}
	else {
		return {};
	}
}

String LuaCallable::get_as_text() const {
	return String("<LuaCallable %s>") % _lua_func->to_string();
}

uint32_t LuaCallable::hash() const {
	return get_as_text().hash();
}

void LuaCallable::call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, GDExtensionCallError &r_call_error) const {
	r_return_value =  _lua_func->invoke(p_arguments, p_argcount, r_call_error);
}

Variant LuaCallable::construct(sol::function func) {
	return Variant{Callable{memnew(LuaCallable(func))}};
};

bool LuaCallable::compare_equal_func(const CallableCustom* p_a, const CallableCustom* p_b) {
	const LuaCallable* a = static_cast<const LuaCallable*>(p_a);
	const LuaCallable* b = static_cast<const LuaCallable*>(p_b);
	return a->_lua_func == b->_lua_func;
}

bool LuaCallable::compare_less_func(const CallableCustom* p_a, const CallableCustom* p_b) {
	const LuaCallable* a = static_cast<const LuaCallable*>(p_a);
	const LuaCallable* b = static_cast<const LuaCallable*>(p_b);
	return a->_lua_func < b->_lua_func;
}

}
