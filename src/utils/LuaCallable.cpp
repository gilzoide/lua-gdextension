#include "LuaCallable.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/variant.hpp"

namespace luagdextension {

LuaCallable::CompareEqualFunc LuaCallable::get_compare_equal_func() const {
	return nullptr;
}

LuaCallable::CompareLessFunc LuaCallable::get_compare_less_func() const {
	return nullptr;
}

bool LuaCallable::is_valid() const {
	return _lua_func->get_function().valid();
}

ObjectID LuaCallable::get_object() const {
	return {};
}

String LuaCallable::get_as_text() const {
	return "<LuaCallable>";
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

}
