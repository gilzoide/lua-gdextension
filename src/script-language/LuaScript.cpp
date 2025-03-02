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
#include "LuaScript.hpp"

#include "LuaScriptInstance.hpp"
#include "LuaScriptLanguage.hpp"
#include "../LuaError.hpp"
#include "../LuaFunction.hpp"
#include "../LuaState.hpp"
#include "../LuaTable.hpp"

#include "gdextension_interface.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/godot.hpp"

namespace luagdextension {

bool LuaScript::_editor_can_reload_from_file() {
	return true;
}

void LuaScript::_placeholder_erased(void *p_placeholder) {
	// TODO
}

bool LuaScript::_can_instantiate() const {
	return metatable.is_valid()
		&& ClassDB::can_instantiate(_get_instance_base_type());
}

Ref<Script> LuaScript::_get_base_script() const {
	return {};
}

StringName LuaScript::_get_global_name() const {
	if (metatable.is_valid()) {
		return metatable->get_value("class_name");
	}
	else {
		return {};
	}
}

bool LuaScript::_inherits_script(const Ref<Script> &script) const {
	return false;
}

StringName LuaScript::_get_instance_base_type() const {
	if (metatable.is_valid()) {
		return metatable->get_value("extends", RefCounted::get_class_static());
	}
	else {
		return RefCounted::get_class_static();
	}
}

void *LuaScript::_instance_create(Object *for_object) const {
	void *instance = memnew(LuaScriptInstance(for_object, Ref<LuaScript>(this)));
	return godot::internal::gdextension_interface_script_instance_create3(LuaScriptInstance::get_script_instance_info(), instance);
}

void *LuaScript::_placeholder_instance_create(Object *for_object) const {
	return godot::internal::gdextension_interface_placeholder_script_instance_create(LuaScriptLanguage::get_singleton(), (Object *) this, for_object);
}

bool LuaScript::_instance_has(Object *p_object) const {
	return LuaScriptInstance::attached_to_object(p_object);
}

bool LuaScript::_has_source_code() const {
	return !source_code.is_empty();
}

String LuaScript::_get_source_code() const {
	return source_code;
}

void LuaScript::_set_source_code(const String &code) {
	source_code = code;
}

Error LuaScript::_reload(bool keep_state) {
	Variant result = LuaScriptLanguage::get_singleton()->get_lua_state()->do_string(source_code, get_path());
	if (LuaError *error = Object::cast_to<LuaError>(result)) {
		ERR_PRINT(error->get_message());
		switch (error->get_status()) {
			case LuaError::MEMORY:
			case LuaError::GC:
				return ERR_OUT_OF_MEMORY;

			case LuaError::SYNTAX:
				// Let editor load the script, so we can edit it
				if (Engine::get_singleton()->is_editor_hint()) {
					break;
				}
				return ERR_PARSE_ERROR;

			default:
				return ERR_SCRIPT_FAILED;
		}
	}
	process_script_result(result);
	return OK;
}

TypedArray<Dictionary> LuaScript::_get_documentation() const {
	// TODO
	return {};
}

String LuaScript::_get_class_icon_path() const {
	if (metatable.is_valid()) {
		return metatable->get_value("icon");
	}
	else {
		return {};
	}
}

bool LuaScript::_has_method(const StringName &p_method) const {
	return methods.has(p_method);
}

bool LuaScript::_has_static_method(const StringName &p_method) const {
	// In Lua, all methods can be called as static methods
	// Pass "self" manually if necessary
	return _has_method(p_method);
}

Variant LuaScript::_get_script_method_argument_count(const StringName &p_method) const {
	// TODO
	return {};
}

Dictionary LuaScript::_get_method_info(const StringName &p_method) const {
	// TODO
	return {};
}

bool LuaScript::_is_tool() const {
	if (metatable.is_valid()) {
		return metatable->get_value("tool");
	}
	else {
		return false;
	}
}

bool LuaScript::_is_valid() const {
	return true;
}

bool LuaScript::_is_abstract() const {
	return false;
}

ScriptLanguage *LuaScript::_get_language() const {
	return LuaScriptLanguage::get_singleton();
}

bool LuaScript::_has_script_signal(const StringName &p_signal) const {
	return signals.has(p_signal);
}

TypedArray<Dictionary> LuaScript::_get_script_signal_list() const {
	// TODO
	return {};
}

bool LuaScript::_has_property_default_value(const StringName &p_property) const {
	// TODO
	return {};
}

Variant LuaScript::_get_property_default_value(const StringName &p_property) const {
	// TODO
	return {};
}

void LuaScript::_update_exports() {
	// TODO
}

TypedArray<Dictionary> LuaScript::_get_script_method_list() const {
	// TODO
	return {};
}

TypedArray<Dictionary> LuaScript::_get_script_property_list() const {
	// TODO
	return {};
}

int32_t LuaScript::_get_member_line(const StringName &p_member) const {
	// TODO
	return {};
}

Dictionary LuaScript::_get_constants() const {
	// TODO
	return {};
}

TypedArray<StringName> LuaScript::_get_members() const {
	// TODO
	return {};
}

bool LuaScript::_is_placeholder_fallback_enabled() const {
	// TODO
	return {};
}

Variant LuaScript::_get_rpc_config() const {
	// TODO
	return {};
}

Variant LuaScript::_new(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
	if (!_can_instantiate()) {
		error.error = GDEXTENSION_CALL_ERROR_INVALID_METHOD;
		return {};
	}

	Variant new_instance = ClassDB::instantiate(_get_instance_base_type());
	if (Object *obj = new_instance) {
		obj->set_script(this);
	}
	return new_instance;
}

bool LuaScript::_instance_set(LuaScriptInstance *instance, const StringName& p_name, const Variant& p_value) const {
	if (const Ref<LuaFunction> *_set = methods.getptr("_set"); _set && _set->is_valid()) {
		Variant result = _set->ptr()->call_method(instance, p_name, p_value);
		if (LuaError *lua_error = Object::cast_to<LuaError>(result)) {
			ERR_FAIL_V_MSG(false, lua_error->get_message());
		}
		return true;
	}

	if (const LuaScriptProperty *lua_prop = properties.getptr(p_name)) {
		if (lua_prop->setter.is_valid()) {
			lua_prop->setter->call_method(instance, p_value);
		}
		else {
			instance->properties[p_name] = p_value;
		}
		return true;
	}
	else {
		return false;
	}
}

bool LuaScript::_instance_get(LuaScriptInstance *instance, const StringName& p_name, Variant& p_value) const {
	if (const Ref<LuaFunction> *_get = methods.getptr("_get"); _get && _get->is_valid()) {
		p_value = _get->ptr()->call_method(instance, p_name);
		if (LuaError *lua_error = Object::cast_to<LuaError>(p_value)) {
			ERR_FAIL_V_MSG(false, lua_error->get_message());
		}
		if (p_value != Variant()) {
			return true;
		}
	}

	if (const LuaScriptProperty *lua_prop = properties.getptr(p_name)) {
		if (lua_prop->getter.is_valid()) {
			p_value = lua_prop->getter->call_method(instance);
		}
		else {
			p_value = instance->properties.get(p_name, lua_prop->default_value);
		}
		return true;
	}
	else if (instance->properties.has(p_name)) {
		p_value = instance->properties[p_name];
		return true;
	}
	else {
		return false;
	}
}

Variant LuaScript::_instance_call(LuaScriptInstance *instance, const StringName& p_name, const Variant **p_args, GDExtensionInt p_argument_count, GDExtensionCallError& r_error) const {
	if (const Ref<LuaFunction> *method = methods.getptr(p_name)) {
		return method->ptr()->invoke_method(instance, p_args, p_argument_count, r_error);
	}
	else {
		r_error.error = GDEXTENSION_CALL_ERROR_INVALID_METHOD;
	}
	return {};
}

void LuaScript::_instance_notification(LuaScriptInstance *instance, int32_t what, GDExtensionBool reversed) const {
	if (const Ref<LuaFunction> *method = methods.getptr("_notification")) {
		Variant result = method->ptr()->call_method(instance, what, reversed);
		if (LuaError *lua_error = Object::cast_to<LuaError>(result)) {
			ERR_FAIL_MSG(lua_error->get_message());
		}
	}
}

bool LuaScript::_instance_tostring(LuaScriptInstance *instance, String& str) const {
	if (const Ref<LuaFunction> *method = methods.getptr("__tostring")) {
		Variant result = method->ptr()->call_method(instance);
		if (LuaError *lua_error = Object::cast_to<LuaError>(result)) {
			ERR_FAIL_V_MSG(false, lua_error->get_message());
		}
		str = result;
		return true;
	}
	else {
		return false;
	}
}

void LuaScript::_bind_methods() {
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "new", &LuaScript::_new);
}

String LuaScript::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

void LuaScript::process_script_result(const Variant& result) {
	metatable.reference_ptr(Object::cast_to<LuaTable>(result));
	methods.clear();
	properties.clear();
	signals.clear();
	if (!metatable.is_valid()) {
		return;
	}

	for (Variant key : *metatable.ptr()) {
		// skip special keys, they are not considered properties
		if (key.in(Array::make("class_name", "extends", "icon", "tool"))) {
			continue;
		}

		// methods, signals and properties must have string key
		if (key.get_type() != Variant::STRING && key.get_type() != Variant::STRING_NAME) {
			continue;
		}

		Variant value = metatable->get_value(key);
		switch (value.get_type()) {
			case Variant::SIGNAL:
				signals[key] = value;
				break;

			case Variant::OBJECT:
				if (LuaFunction *method = Object::cast_to<LuaFunction>(value)) {
					methods[key].reference_ptr(method);
				}
				// fallthrough
			default:
				// TODO: add support for property metadata
				properties[key] = LuaScriptProperty {
					.type = value.get_type(),
					.default_value = value,
				};
				break;
		}
	}
}

}

