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
#include "../LuaTable.hpp"

namespace luagdextension {

LuaScriptInstance::LuaScriptInstance(Object *owner, Ref<LuaScript> script)
	: owner(owner)
	, script(script)
	, data(LuaScriptLanguage::get_singleton()->get_lua_state()->create_table())
{
	known_instances.insert(owner, this);
}

LuaScriptInstance::~LuaScriptInstance() {
	known_instances.erase(owner);
}

GDExtensionBool set_func(LuaScriptInstance *p_instance, const StringName *p_name, const Variant *p_value) {
	// try `_set`
	Variant _set = p_instance->script->get_metatable()->get("_set");
	if (LuaFunction *method = Object::cast_to<LuaFunction>(_set)) {
		Variant value_was_set = method->try_call_method(p_instance, *p_name, *p_value);
		if (value_was_set) {
			return true;
		}
	}

	// try setting Object property
	if (ClassDB::class_set_property(p_instance->owner, *p_name, *p_value) == OK) {
		return true;
	}

	p_instance->data->set(*p_name, *p_value);
	return true;
}

GDExtensionBool get_func(LuaScriptInstance *p_instance, const StringName *p_name, Variant *p_value) {
	// call `_get`
	Variant _get = p_instance->script->get_metatable()->get("_get");
	if (LuaFunction *method = Object::cast_to<LuaFunction>(_get)) {
		Variant value = method->try_call_method(p_instance, *p_name);
		if (value != Variant()) {
			*p_value = value;
			return true;
		}
	}

	// access own data
	if (auto data_value = p_instance->data->try_get(*p_name)) {
		*p_value = *data_value;
		return true;
	}

	// fallback to value in script
	if (auto script_value = p_instance->script->get_metatable()->try_get(*p_name)) {
		*p_value = *script_value;
		return true;
	}

	return false;
}

GDExtensionScriptInstanceGetPropertyList get_property_list_func;
GDExtensionScriptInstanceFreePropertyList2 free_property_list_func;
GDExtensionScriptInstanceGetClassCategory get_class_category_func;

GDExtensionScriptInstancePropertyCanRevert property_can_revert_func;
GDExtensionScriptInstancePropertyGetRevert property_get_revert_func;

Object *get_owner_func(LuaScriptInstance *p_instance) {
	return p_instance->owner;
}

GDExtensionScriptInstanceGetPropertyState get_property_state_func;

GDExtensionScriptInstanceGetMethodList get_method_list_func;
GDExtensionScriptInstanceFreeMethodList2 free_method_list_func;
GDExtensionScriptInstanceGetPropertyType get_property_type_func;
GDExtensionScriptInstanceValidateProperty validate_property_func;

GDExtensionBool has_method_func(LuaScriptInstance *p_instance, const StringName *p_name) {
	return p_instance->script->_has_method(*p_name);
}
GDExtensionScriptInstanceGetMethodArgumentCount get_method_argument_count_func;

void call_func(LuaScriptInstance *p_instance, const StringName *p_method, const Variant **p_args, GDExtensionInt p_argument_count, Variant *r_return, GDExtensionCallError *r_error) {
	if (*p_method == StringName("rawget")) {
		if (p_argument_count == 1) {
			r_error->error = GDEXTENSION_CALL_OK;
			*r_return = p_instance->data->rawget(*p_args[0]);
		}
		else if (p_argument_count < 1) {
			r_error->error = GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error->expected = 1;
		}
		else {
			r_error->error = GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error->expected = 1;
		}
		return;
	}
	if (*p_method == StringName("rawset")) {
		if (p_argument_count == 2) {
			r_error->error = GDEXTENSION_CALL_OK;
			*r_return = Variant();
			p_instance->data->rawset(*p_args[0], *p_args[1]);
		}
		else if (p_argument_count < 2) {
			r_error->error = GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error->expected = 2;
		}
		else {
			r_error->error = GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error->expected = 2;
		}
		return;
	}

	Variant value = p_instance->script->get_metatable()->get(*p_method);
	if (LuaFunction *method = Object::cast_to<LuaFunction>(value)) {
		*r_return = method->invoke_method(p_instance, p_args, p_argument_count, *r_error);
	}
	else {
		r_error->error = GDEXTENSION_CALL_ERROR_INVALID_METHOD;
	}
}

void notification_func(LuaScriptInstance *p_instance, int32_t p_what, GDExtensionBool p_reversed) {
	Variant value = p_instance->script->get_metatable()->get("_notification");
	if (LuaFunction *method = Object::cast_to<LuaFunction>(value)) {
		method->try_call_method(p_instance, p_what, p_reversed);
	}
}

void to_string_func(LuaScriptInstance *p_instance, GDExtensionBool *r_is_valid, String *r_out) {
	Variant value = p_instance->script->get_metatable()->get("_to_string");
	if (LuaFunction *method = Object::cast_to<LuaFunction>(value)) {
		*r_out = method->try_call_method(p_instance);
		*r_is_valid = true;
	}
	else {
		*r_is_valid = false;
	}
}

void refcount_incremented_func(LuaScriptInstance *) {
}

GDExtensionBool refcount_decremented_func(LuaScriptInstance *) {
	return false;
}

Object *get_script_func(LuaScriptInstance *instance) {
	return instance->script.ptr();
}

GDExtensionBool is_placeholder_func(LuaScriptInstance *instance) {
	return false;
}

GDExtensionScriptInstanceSet set_fallback_func;
GDExtensionScriptInstanceGet get_fallback_func;

void *get_language_func(LuaScriptInstance *instance) {
	return LuaScriptLanguage::get_singleton();
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

HashMap<Object *, LuaScriptInstance *> LuaScriptInstance::known_instances;

}
