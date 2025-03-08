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
#include "LuaScriptMethod.hpp"
#include "LuaScriptProperty.hpp"
#include "../LuaError.hpp"
#include "../LuaFunction.hpp"
#include "../LuaState.hpp"
#include "../LuaTable.hpp"
#include "../utils/convert_godot_lua.hpp"

#include "gdextension_interface.h"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/godot.hpp"

namespace luagdextension {

LuaScript::LuaScript()
	: ScriptExtension()
{
	placeholders.insert(this, {});
}

LuaScript::~LuaScript() {
	placeholders.erase(this);
}

bool LuaScript::_editor_can_reload_from_file() {
	return true;
}

void LuaScript::_placeholder_erased(void *p_placeholder) {
	placeholders.get(this).erase(p_placeholder);
}

bool LuaScript::_can_instantiate() const {
	return _is_valid() && (_is_tool() || !Engine::get_singleton()->is_editor_hint());
}

Ref<Script> LuaScript::_get_base_script() const {
	return {};
}

StringName LuaScript::_get_global_name() const {
	return metadata.class_name;
}

bool LuaScript::_inherits_script(const Ref<Script> &script) const {
	return false;
}

StringName LuaScript::_get_instance_base_type() const {
	return metadata.base_class;
}

void *LuaScript::_instance_create(Object *for_object) const {
	LuaScriptInstance *instance = memnew(LuaScriptInstance(for_object, Ref<LuaScript>(this)));
	return godot::internal::gdextension_interface_script_instance_create3(LuaScriptInstance::get_script_instance_info(), instance);
}

void *LuaScript::_placeholder_instance_create(Object *for_object) const {
	void *placeholder = godot::internal::gdextension_interface_placeholder_script_instance_create(LuaScriptLanguage::get_singleton()->_owner, this->_owner, for_object->_owner);
	placeholders.get(this).insert(placeholder);
	_update_placeholder_exports(placeholder);
	return placeholder;
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
	_reload(true);
}

Error LuaScript::_reload(bool keep_state) {
	placeholder_fallback_enabled = true;
	metadata.clear();

	Variant result = LuaScriptLanguage::get_singleton()->get_lua_state()->do_string(source_code, get_path());
	if (LuaError *error = Object::cast_to<LuaError>(result)) {
		return ERR_PARSE_ERROR;
	}
	else if (LuaTable *table = Object::cast_to<LuaTable>(result)) {
		placeholder_fallback_enabled = false;
		metadata.setup(table->get_table());
	}
	return OK;
}

TypedArray<Dictionary> LuaScript::_get_documentation() const {
	// TODO
	return {};
}

String LuaScript::_get_class_icon_path() const {
	return metadata.icon_path;
}

bool LuaScript::_has_method(const StringName &p_method) const {
	return metadata.methods.has(p_method);
}

bool LuaScript::_has_static_method(const StringName &p_method) const {
	// In Lua, all methods can be called as static methods
	// Pass "self" manually if necessary
	return _has_method(p_method);
}

Variant LuaScript::_get_script_method_argument_count(const StringName &p_method) const {
	if (const LuaScriptMethod *method = metadata.methods.getptr(p_method)) {
		return method->get_argument_count();
	}
	else {
		return {};
	}
}

Dictionary LuaScript::_get_method_info(const StringName &p_method) const {
	if (const LuaScriptMethod *method = metadata.methods.getptr(p_method)) {
		return method->to_dictionary();
	}
	else {
		return {};
	}
}

bool LuaScript::_is_tool() const {
	return metadata.is_tool;
}

bool LuaScript::_is_valid() const {
	return metadata.is_valid;
}

bool LuaScript::_is_abstract() const {
	return false;
}

ScriptLanguage *LuaScript::_get_language() const {
	return LuaScriptLanguage::get_singleton();
}

bool LuaScript::_has_script_signal(const StringName &p_signal) const {
	return metadata.signals.has(p_signal);
}

TypedArray<Dictionary> LuaScript::_get_script_signal_list() const {
	TypedArray<Dictionary> signals;
	for (auto [name, signal] : metadata.signals) {
		signals.append(signal.to_dictionary());
	}
	return signals;
}

bool LuaScript::_has_property_default_value(const StringName &p_property) const {
	return metadata.properties.has(p_property);
}

Variant LuaScript::_get_property_default_value(const StringName &p_property) const {
	if (const LuaScriptProperty *property = metadata.properties.getptr(p_property)) {
		return property->default_value;
	}
	else {
		return {};
	}
}

void LuaScript::_update_exports() {
	for (void *placeholder : placeholders.get(this)) {
		_update_placeholder_exports(placeholder);
	}
}

TypedArray<Dictionary> LuaScript::_get_script_method_list() const {
	TypedArray<Dictionary> methods;
	for (auto [name, method] : metadata.methods) {
		methods.append(method.to_dictionary());
	}
	return methods;
}

TypedArray<Dictionary> LuaScript::_get_script_property_list() const {
	TypedArray<Dictionary> list;
	for (auto [name, prop] : metadata.properties) {
		list.append(prop.to_dictionary());
	}
	return list;
}

int32_t LuaScript::_get_member_line(const StringName &p_member) const {
#ifdef DEBUG_ENABLED
	if (const LuaScriptMethod *method = metadata.methods.getptr(p_member)) {
		return method->get_line_defined();
	}
#endif
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
	return placeholder_fallback_enabled;
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
	if (const LuaScriptMethod *_init = metadata.methods.getptr("_init")) {
		LuaFunction::invoke_method_lua(_init->method, new_instance, args, arg_count, false);
	}
	return new_instance;
}

const LuaScriptMetadata& LuaScript::get_metadata() const {
	return metadata;
}

void LuaScript::_bind_methods() {
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "new", &LuaScript::_new);
}

String LuaScript::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

void LuaScript::_update_placeholder_exports(void *placeholder) const {
	Array properties;
	Dictionary default_values;
	for (auto [name, property] : metadata.properties) {
		properties.append(property.to_dictionary());
		default_values[name] = property.instantiate_default_value();
	}
	godot::internal::gdextension_interface_placeholder_script_instance_update(placeholder, properties._native_ptr(), default_values._native_ptr());
}

HashMap<const LuaScript *, HashSet<void *>> LuaScript::placeholders;

}

