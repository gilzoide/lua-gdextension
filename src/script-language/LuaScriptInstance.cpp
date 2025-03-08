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
#include <godot_cpp/classes/script.hpp>

#include "LuaScriptInstance.hpp"

#include "LuaScript.hpp"
#include "LuaScriptLanguage.hpp"
#include "LuaScriptMetadata.hpp"
#include "../LuaError.hpp"
#include "../LuaFunction.hpp"
#include "../LuaTable.hpp"
#include "../utils/convert_godot_lua.hpp"

namespace luagdextension {

LuaScriptInstance::LuaScriptInstance(Object *owner, Ref<LuaScript> script)
	: owner(owner)
	, script(script)
	, data(LuaScriptLanguage::get_singleton()->get_lua_state()->create_table())
{
	known_instances.insert(owner, this);

	const LuaScriptMetadata& metadata = script->get_metadata();
	for (auto [name, signal] : metadata.signals) {
		data->set(name, Signal(owner, name));
	}
}

LuaScriptInstance::~LuaScriptInstance() {
	known_instances.erase(owner);
}

GDExtensionBool set_func(LuaScriptInstance *p_instance, const StringName *p_name, const Variant *p_value) {
	// 1) try calling `_set`
	if (const sol::protected_function *_set = p_instance->script->get_metadata().methods.getptr("_set")) {
		Variant value_was_set = LuaFunction::invokev_lua(*_set, Array::make(p_instance->owner, *p_name, *p_value), false);
		if (value_was_set) {
			return true;
		}
	}

	// b) try setter function from script property
	const LuaScriptProperty *property = p_instance->script->get_metadata().properties.getptr(*p_name);
	if (property && property->set_value(p_instance, *p_value)) {
		return true;
	}

	// c) try setting owner Object property
	if (ClassDB::class_set_property(p_instance->owner, *p_name, *p_value) == OK) {
		return true;
	}

	// d) set raw data
	p_instance->data->rawset(*p_name, *p_value);
	return true;
}

GDExtensionBool get_func(LuaScriptInstance *p_instance, const StringName *p_name, Variant *p_value) {
	// a) try calling `_get`
	if (const sol::protected_function *_get = p_instance->script->get_metadata().methods.getptr("_get")) {
		Variant value = LuaFunction::invokev_lua(*_get, Array::make(p_instance->owner, *p_name), false);
		if (value != Variant()) {
			*p_value = value;
			return true;
		}
	}

	// b) try getter function from script property
	const LuaScriptProperty *property = p_instance->script->get_metadata().properties.getptr(*p_name);
	if (property && property->get_value(p_instance, *p_value)) {
		return true;
	}

	// c) access raw data
	if (auto data_value = p_instance->data->try_get(*p_name)) {
		*p_value = *data_value;
		return true;
	}

	// d) fallback to default property value, if there is one
	if (property) {
		Variant value = property->instantiate_default_value();
		p_instance->data->rawset(*p_name, value);
		*p_value = value;
		return true;
	}

	// e) for methods, return a bound Callable
	if (p_instance->script->get_metadata().methods.has(*p_name)) {
		*p_value = Callable(p_instance->owner, *p_name);
		return true;
	}

	return false;
}

GDExtensionScriptInstanceGetPropertyList get_property_list_func;
GDExtensionScriptInstanceFreePropertyList2 free_property_list_func;
GDExtensionScriptInstanceGetClassCategory get_class_category_func;

GDExtensionBool property_can_revert_func(LuaScriptInstance *p_instance, const StringName *p_name) {
	if (const sol::protected_function *method = p_instance->script->get_metadata().methods.getptr("_property_can_revert")) {
		Variant result = LuaFunction::invokev_lua(*method, Array::make(p_instance->owner, *p_name), false);
		if (result) {
			return true;
		}
	}

	return false;
}

GDExtensionBool property_get_revert_func(LuaScriptInstance *p_instance, const StringName *p_name, Variant *r_ret) {
	if (const sol::protected_function *method = p_instance->script->get_metadata().methods.getptr("_property_get_revert")) {
		Variant result = LuaFunction::invokev_lua(*method, Array::make(p_instance->owner, *p_name), true);
		if (LuaError *error = Object::cast_to<LuaError>(result)) {
			ERR_PRINT(error->get_message());
		}
		else {
			*r_ret = result;
			return true;
		}
	}

	return false;
}

Object *get_owner_func(LuaScriptInstance *p_instance) {
	return p_instance->owner;
}

void get_property_state_func(LuaScriptInstance *p_instance, GDExtensionScriptInstancePropertyStateAdd p_add_func, void *p_userdata) {
	for (Variant key : *p_instance->data.ptr()) {
		StringName name = key;
		Variant value = p_instance->data->get(key);
		p_add_func(&name, &value, p_userdata);
	}
}

GDExtensionScriptInstanceGetMethodList get_method_list_func;
GDExtensionScriptInstanceFreeMethodList2 free_method_list_func;
GDExtensionScriptInstanceGetPropertyType get_property_type_func;
GDExtensionScriptInstanceValidateProperty validate_property_func;

GDExtensionBool has_method_func(LuaScriptInstance *p_instance, const StringName *p_name) {
	return p_instance->script->_has_method(*p_name);
}
GDExtensionScriptInstanceGetMethodArgumentCount get_method_argument_count_func;

void call_func(LuaScriptInstance *p_instance, const StringName *p_method, const Variant **p_args, GDExtensionInt p_argument_count, Variant *r_return, GDExtensionCallError *r_error) {
	if (const sol::protected_function *method = p_instance->script->get_metadata().methods.getptr(*p_method)) {
		r_error->error = GDEXTENSION_CALL_OK;
		*r_return = LuaFunction::invoke_method_lua(*method, p_instance->owner, p_args, p_argument_count, false);
	}
	else {
		r_error->error = GDEXTENSION_CALL_ERROR_INVALID_METHOD;
	}
}

void notification_func(LuaScriptInstance *p_instance, int32_t p_what, GDExtensionBool p_reversed) {
	if (const sol::protected_function *method = p_instance->script->get_metadata().methods.getptr("_notification")) {
		LuaFunction::invokev_lua(*method, Array::make(p_instance->owner, p_what, p_reversed), false);
	}
}

void to_string_func(LuaScriptInstance *p_instance, GDExtensionBool *r_is_valid, String *r_out) {
	if (const sol::protected_function *method = p_instance->script->get_metadata().methods.getptr("_to_string")) {
		Variant result = LuaFunction::invokev_lua(*method, Array::make(p_instance->owner), false);
		if (result) {
			*r_out = result;
			*r_is_valid = true;
		}
		else {
			*r_is_valid = false;
		}
	}
	else {
		*r_is_valid = false;
	}
}

void refcount_incremented_func(LuaScriptInstance *) {
}

GDExtensionBool refcount_decremented_func(LuaScriptInstance *) {
	return true;
}

void *get_script_func(LuaScriptInstance *instance) {
	return instance->script.ptr()->_owner;
}

GDExtensionBool is_placeholder_func(LuaScriptInstance *instance) {
	return false;
}

GDExtensionScriptInstanceSet set_fallback_func;
GDExtensionScriptInstanceGet get_fallback_func;

void *get_language_func(LuaScriptInstance *instance) {
	return LuaScriptLanguage::get_singleton()->_owner;
}

void free_func(LuaScriptInstance *instance) {
	memdelete(instance);
}

GDExtensionScriptInstanceInfo3 script_instance_info = {
	(GDExtensionScriptInstanceSet) set_func,
	(GDExtensionScriptInstanceGet) get_func,
	(GDExtensionScriptInstanceGetPropertyList) get_property_list_func,
	(GDExtensionScriptInstanceFreePropertyList2) free_property_list_func,
	(GDExtensionScriptInstanceGetClassCategory) get_class_category_func,
	(GDExtensionScriptInstancePropertyCanRevert) property_can_revert_func,
	(GDExtensionScriptInstancePropertyGetRevert) property_get_revert_func,
	(GDExtensionScriptInstanceGetOwner) get_owner_func,
	(GDExtensionScriptInstanceGetPropertyState) get_property_state_func,
	(GDExtensionScriptInstanceGetMethodList) get_method_list_func,
	(GDExtensionScriptInstanceFreeMethodList2) free_method_list_func,
	(GDExtensionScriptInstanceGetPropertyType) get_property_type_func,
	(GDExtensionScriptInstanceValidateProperty) validate_property_func,
	(GDExtensionScriptInstanceHasMethod) has_method_func,
	(GDExtensionScriptInstanceGetMethodArgumentCount) get_method_argument_count_func,
	(GDExtensionScriptInstanceCall) call_func,
	(GDExtensionScriptInstanceNotification2) notification_func,
	(GDExtensionScriptInstanceToString) to_string_func,
	(GDExtensionScriptInstanceRefCountIncremented) refcount_incremented_func,
	(GDExtensionScriptInstanceRefCountDecremented) refcount_decremented_func,
	(GDExtensionScriptInstanceGetScript) get_script_func,
	(GDExtensionScriptInstanceIsPlaceholder) is_placeholder_func,
	(GDExtensionScriptInstanceSet) set_fallback_func,
	(GDExtensionScriptInstanceGet) get_fallback_func,
	(GDExtensionScriptInstanceGetLanguage) get_language_func,
	(GDExtensionScriptInstanceFree) free_func,
};
GDExtensionScriptInstanceInfo3 *LuaScriptInstance::get_script_instance_info() {
	return &script_instance_info;
}

LuaScriptInstance *LuaScriptInstance::attached_to_object(Object *owner) {
	if (LuaScriptInstance **ptr = known_instances.getptr(owner)) {
		return *ptr;
	}
	else {
		return nullptr;
	}
}

Variant LuaScriptInstance::rawget(const Variant& self, const Variant& index) {
	if (LuaScriptInstance *instance = attached_to_object(self)) {
		return instance->data->rawget(index);
	}
	else {
		return {};
	}
}

void LuaScriptInstance::rawset(const Variant& self, const Variant& index, const Variant& value) {
	if (LuaScriptInstance *instance = attached_to_object(self)) {
		instance->data->rawset(index, value);
	}
}

HashMap<Object *, LuaScriptInstance *> LuaScriptInstance::known_instances;

}
