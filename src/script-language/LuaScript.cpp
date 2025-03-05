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
	_reload(true);
}

Error LuaScript::_reload(bool keep_state) {
	metatable.unref();

	Variant result = LuaScriptLanguage::get_singleton()->get_lua_state()->do_string(source_code, get_path());
	if (LuaError *error = Object::cast_to<LuaError>(result)) {
		ERR_PRINT(error->get_message());
	}
	else {
		process_script_result(result);
	}
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
	if (metatable.is_valid()) {
		Variant value = metatable->get_value(p_method);
		LuaFunction *method = Object::cast_to<LuaFunction>(value);
		return method != nullptr;
	}
	else {
		return false;
	}
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
	return metatable.is_valid();
}

bool LuaScript::_is_abstract() const {
	return false;
}

ScriptLanguage *LuaScript::_get_language() const {
	return LuaScriptLanguage::get_singleton();
}

bool LuaScript::_has_script_signal(const StringName &p_signal) const {
	if (metatable.is_valid()) {
		Variant value = metatable->get_value(p_signal);
		return value.get_type() == Variant::Type::SIGNAL;
	}
	else {
		return false;
	}
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

Ref<LuaTable> LuaScript::get_metatable() {
	return metatable;
}

void LuaScript::_bind_methods() {
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "new", &LuaScript::_new);
}

String LuaScript::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

void LuaScript::process_script_result(const Variant& result) {
	LuaTable *table = Object::cast_to<LuaTable>(result);
	if (!table) {
		return;
	}

	metatable.reference_ptr(table);

	// Remove "extends" field if it's invalid
	Variant base_class = table->get_value("extends");
	if (base_class && !ClassDB::class_exists(base_class)) {
		WARN_PRINT(String("Specified base class '%s' does not exist. Unsetting 'extends'") % Array::make(base_class));
		table->set_value("extends", Variant());
	}
}

}

