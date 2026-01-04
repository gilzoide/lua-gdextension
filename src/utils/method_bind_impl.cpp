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
#include "method_bind_impl.hpp"

#include "VariantArguments.hpp"
#include "convert_godot_lua.hpp"
#include "string_names.hpp"
#include "../LuaTable.hpp"

#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

BaseMethodBind::BaseMethodBind(const StringName& method_name)
	: method_name(method_name)
{
}

const StringName& BaseMethodBind::get_method_name() const {
	return method_name;
}


// ClassMethodBind
ClassMethodBind::ClassMethodBind(const Class& cls, const StringName& method_name)
	: BaseMethodBind(method_name)
	, cls(cls)
{
}

sol::object ClassMethodBind::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	ERR_FAIL_COND_V_MSG(!self.is<Class>() || self.as<Class&>() != cls, sol::nil, String("To call methods in Lua, use ':' instead of '.': `Class:%s(...)`") % method_name);
	Array var_args = VariantArguments(args).get_array();
	var_args.push_front(get_method_name());
	var_args.push_front(cls.get_name());
	return to_lua(state, ClassDBSingleton::get_singleton()->callv(string_names->class_call_static, var_args));
}

void ClassMethodBind::register_usertype(sol::state_view& state) {
	BaseMethodBind::register_subtype<ClassMethodBind>(state, "ClassMethodBind");
}


// LuaScriptInstanceMethodBind
LuaScriptInstanceMethodBind::LuaScriptInstanceMethodBind(LuaScriptInstance *instance, const StringName& method_name)
	: BaseMethodBind(method_name)
	, instance_owner(instance->owner)
{
}

Callable LuaScriptInstanceMethodBind::to_callable() const {
	LuaScriptInstance *script_instance = LuaScriptInstance::attached_to_object(instance_owner);
	ERR_FAIL_COND_V_MSG(script_instance == nullptr, Callable(), "Lua script instance is no longer valid");
	return Callable(instance_owner, method_name);
}

sol::object LuaScriptInstanceMethodBind::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	LuaScriptInstance *script_instance = LuaScriptInstance::attached_to_object(instance_owner);
	ERR_FAIL_COND_V_MSG(script_instance == nullptr, sol::nil, "Lua script instance is no longer valid");
	ERR_FAIL_COND_V_MSG(self.pointer() != script_instance->data->get_table().pointer(), sol::nil, String("To call methods in Lua, use ':' instead of '.': `self:%s(...)`") % method_name);
	Variant v = instance_owner;
	return variant_call_string_name(state, v, method_name, args);
}

void LuaScriptInstanceMethodBind::register_usertype(sol::state_view& state) {
	BaseMethodBind::register_subtype<LuaScriptInstanceMethodBind>(state, "LuaScriptInstanceMethodBind");
}


// VariantMethodBind
VariantMethodBind::VariantMethodBind(const Variant& variant, const StringName& method_name)
	: BaseMethodBind(method_name)
	, variant(variant)
{
}

Callable VariantMethodBind::to_callable() const {
	return Callable::create(variant, method_name);
}

sol::object VariantMethodBind::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	Variant v = to_variant(self);
	ERR_FAIL_COND_V_MSG(!UtilityFunctions::is_same(v, variant), sol::nil, String("To call methods in Lua, use ':' instead of '.': `variant:%s(...)`") % method_name);
	return variant_call_string_name(state, v, method_name, args);
}

void VariantMethodBind::register_usertype(sol::state_view& state) {
	BaseMethodBind::register_subtype<VariantMethodBind>(state, "VariantMethodBind");
}


// VariantTypeMethodBind
VariantTypeMethodBind::VariantTypeMethodBind(const VariantType& type, const StringName& method_name)
	: BaseMethodBind(method_name)
	, type(type)
{
}

sol::object VariantTypeMethodBind::call(sol::this_state state, const sol::stack_object& self, const sol::variadic_args& args) const {
	if (self.is<VariantType>()) {
		return variant_static_call_string_name(state, type.get_type(), method_name, args);
	}
	else {
		Variant v = to_variant(self);
		ERR_FAIL_COND_V_MSG(v.get_type() != type.get_type(), sol::nil, String("Trying to call a %s method using a value of type %s") % Array::make(Variant::get_type_name(type.get_type()), Variant::get_type_name(v.get_type())));
		return variant_call_string_name(state, v, method_name, args);
	}
}

void VariantTypeMethodBind::register_usertype(sol::state_view& state) {
	BaseMethodBind::register_subtype<VariantTypeMethodBind>(state, "VariantTypeMethodBind");
}

}
