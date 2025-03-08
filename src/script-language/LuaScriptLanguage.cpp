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
#include "LuaScriptLanguage.hpp"

#include "LuaScript.hpp"
#include "LuaScriptProperty.hpp"
#include "LuaScriptSignal.hpp"
#include "../LuaError.hpp"
#include "../LuaTable.hpp"
#include "../LuaState.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/reg_ex.hpp>
#include <godot_cpp/classes/reg_ex_match.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace luagdextension {

String LuaScriptLanguage::_get_name() const {
	return "Lua";
}

void LuaScriptLanguage::_init() {
	lua_state.instantiate();
	lua_state->open_libraries();

	// Register scripting specific usertypes
	sol::state_view state = lua_state->get_lua_state();
	state.new_usertype<LuaScriptSignal>(
		"LuaScriptSignal",
		sol::no_construction()
	);
	state.set("signal", &LuaScriptSignal::from_lua);

	LuaScriptProperty::register_lua(state);
}

String LuaScriptLanguage::_get_type() const {
	return "LuaScript";
}

String LuaScriptLanguage::_get_extension() const {
	return "lua";
}

void LuaScriptLanguage::_finish() {
	lua_state.unref();
}

PackedStringArray LuaScriptLanguage::_get_reserved_words() const {
	return godot::helpers::append_all(PackedStringArray(),
		// Lua keywords
		"and", "break", "do", "else", "elseif", "end",
		"false", "for", "function", "goto", "if", "in",
		"local", "nil", "not", "or", "repeat", "return",
		"then", "true", "until", "while",
		// Other remarkable identifiers
#if LUA_VERSION_NUM >= 502
		"_ENV",
#endif
		"self", "_G", "_VERSION"
	);
}

bool LuaScriptLanguage::_is_control_flow_keyword(const String &keyword) const {
	return godot::helpers::append_all(PackedStringArray(),
		"break", "do", "else", "elseif", "end",
		"for", "goto", "if", "in",
		"repeat", "return",
		"then", "until", "while"
	).has(keyword);
}

PackedStringArray LuaScriptLanguage::_get_comment_delimiters() const {
	return godot::helpers::append_all(PackedStringArray(),
		"--", "--[[ ]]"
	);
}

PackedStringArray LuaScriptLanguage::_get_doc_comment_delimiters() const {
	return godot::helpers::append_all(PackedStringArray(),
		"---"
	);
}

PackedStringArray LuaScriptLanguage::_get_string_delimiters() const {
	return godot::helpers::append_all(PackedStringArray(),
		"' '", "\" \"", "[[ ]]", "[=[ ]=]"
	);
}

Ref<Script> LuaScriptLanguage::_make_template(const String &_template, const String &class_name, const String &base_class_name) const {
	Ref<LuaScript> script = memnew(LuaScript);
	String source_code = _template.replace("_BASE_", base_class_name)
		.replace("_CLASS_", class_name)
		.replace("_TS_", "\t");
	script->set_source_code(source_code);
	return script;
}

TypedArray<Dictionary> LuaScriptLanguage::_get_built_in_templates(const StringName &p_object) const {
#ifdef DEBUG_ENABLED
	Dictionary base_template;
	base_template["inherit"] = "Node";
	base_template["id"] = 0;
	base_template["name"] = "default";
	base_template["description"] = "Default template";
	base_template["origin"] = 0;
	base_template["content"] =
R"(local _CLASS_ = {
	extends = _BASE_,
}

return _CLASS_
)";

	return Array::make(base_template);
#else
	return {};
#endif
}

bool LuaScriptLanguage::_is_using_templates() {
	return true;
}

Dictionary LuaScriptLanguage::_validate(const String &script, const String &path, bool validate_functions, bool validate_errors, bool validate_warnings, bool validate_safe_lines) const {
	Dictionary result;
	Variant f = lua_state->load_string(script, path);
	if (LuaError *error = Object::cast_to<LuaError>(f)) {
		auto line_re = RegEx::create_from_string(R":(\d+):");
		auto match = line_re->search(error->get_message());
		Dictionary error_dict;
		error_dict["path"] = path;
		error_dict["line"] = match->get_string().to_int();
		error_dict["column"] = 1;
		error_dict["message"] = error->get_message();
		result["valid"] = false;
		result["errors"] = Array::make(error_dict);
	}
	else {
		result["valid"] = true;
	}
	return result;
}

String LuaScriptLanguage::_validate_path(const String &path) const {
	return "";
}

Object *LuaScriptLanguage::_create_script() const {
	return memnew(LuaScript);
}

bool LuaScriptLanguage::_has_named_classes() const {
	return false;
}

bool LuaScriptLanguage::_supports_builtin_mode() const {
	return true;
}

bool LuaScriptLanguage::_supports_documentation() const {
	// TODO: does it support documentation?
	return false;
}

bool LuaScriptLanguage::_can_inherit_from_file() const {
	return false;
}

int32_t LuaScriptLanguage::_find_function(const String &p_function, const String &p_code) const {
	// TODO
	return -1;
}

String LuaScriptLanguage::_make_function(const String &p_class_name, const String &p_function_name, const PackedStringArray &p_function_args) const {
	// TODO
	return {};
}

bool LuaScriptLanguage::_can_make_function() const {
	// TODO
	return {};
}

Error LuaScriptLanguage::_open_in_external_editor(const Ref<Script> &p_script, int32_t p_line, int32_t p_column) {
	return OK;
}

bool LuaScriptLanguage::_overrides_external_editor() {
	return false;
}

ScriptLanguage::ScriptNameCasing LuaScriptLanguage::_preferred_file_name_casing() const {
	return SCRIPT_NAME_CASING_AUTO;
}

Dictionary LuaScriptLanguage::_complete_code(const String &p_code, const String &p_path, Object *p_owner) const {
	// TODO
	return {};
}

Dictionary LuaScriptLanguage::_lookup_code(const String &p_code, const String &p_symbol, const String &p_path, Object *p_owner) const {
	// TODO
	Dictionary result;
	result["result"] = ERR_UNAVAILABLE;
	result["type"] = Variant::Type::NIL;
	return result;
}

String LuaScriptLanguage::_auto_indent_code(const String &p_code, int32_t p_from_line, int32_t p_to_line) const {
	// TODO
	return p_code;
}

void LuaScriptLanguage::_add_global_constant(const StringName &p_name, const Variant &p_value) {
	lua_state->get_globals()->set(p_name, p_value);
}

void LuaScriptLanguage::_add_named_global_constant(const StringName &p_name, const Variant &p_value) {
	lua_state->get_globals()->set(p_name, p_value);
}

void LuaScriptLanguage::_remove_named_global_constant(const StringName &p_name) {
	lua_state->get_globals()->set(p_name, nullptr);
}

void LuaScriptLanguage::_thread_enter() {
}

void LuaScriptLanguage::_thread_exit() {
}

String LuaScriptLanguage::_debug_get_error() const {
	// TODO
	return {};
}

int32_t LuaScriptLanguage::_debug_get_stack_level_count() const {
	// TODO
	return {};
}

int32_t LuaScriptLanguage::_debug_get_stack_level_line(int32_t p_level) const {
	// TODO
	return {};
}

String LuaScriptLanguage::_debug_get_stack_level_function(int32_t p_level) const {
	// TODO
	return {};
}

String LuaScriptLanguage::_debug_get_stack_level_source(int32_t p_level) const {
	// TODO
	return {};
}

Dictionary LuaScriptLanguage::_debug_get_stack_level_locals(int32_t p_level, int32_t p_max_subitems, int32_t p_max_depth) {
	// TODO
	return {};
}

Dictionary LuaScriptLanguage::_debug_get_stack_level_members(int32_t p_level, int32_t p_max_subitems, int32_t p_max_depth) {
	// TODO
	return {};
}

void *LuaScriptLanguage::_debug_get_stack_level_instance(int32_t p_level) {
	// TODO
	return {};
}

Dictionary LuaScriptLanguage::_debug_get_globals(int32_t p_max_subitems, int32_t p_max_depth) {
	// TODO
	return {};
}

String LuaScriptLanguage::_debug_parse_stack_level_expression(int32_t p_level, const String &p_expression, int32_t p_max_subitems, int32_t p_max_depth) {
	// TODO
	return {};
}

TypedArray<Dictionary> LuaScriptLanguage::_debug_get_current_stack_info() {
	// TODO
	return {};
}

void LuaScriptLanguage::_reload_all_scripts() {
	// TODO
}

void LuaScriptLanguage::_reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) {
	// TODO
}


PackedStringArray LuaScriptLanguage::_get_recognized_extensions() const {
	return godot::helpers::append_all(PackedStringArray(),
		"lua"
	);
}

TypedArray<Dictionary> LuaScriptLanguage::_get_public_functions() const {
	// TODO
	return {};
}

Dictionary LuaScriptLanguage::_get_public_constants() const {
	// TODO
	return {};
}

TypedArray<Dictionary> LuaScriptLanguage::_get_public_annotations() const {
	// TODO
	return {};
}

void LuaScriptLanguage::_profiling_start() {
	// TODO
}

void LuaScriptLanguage::_profiling_stop() {
	// TODO
}

void LuaScriptLanguage::_profiling_set_save_native_calls(bool p_enable) {
	// TODO
}

int32_t LuaScriptLanguage::_profiling_get_accumulated_data(ScriptLanguageExtensionProfilingInfo *p_info_array, int32_t p_info_max) {
	// TODO
	return {};
}

int32_t LuaScriptLanguage::_profiling_get_frame_data(ScriptLanguageExtensionProfilingInfo *p_info_array, int32_t p_info_max) {
	// TODO
	return {};
}

void LuaScriptLanguage::_frame() {
}

bool LuaScriptLanguage::_handles_global_class_type(const String &type) const {
	return type == _get_type();
}

Dictionary LuaScriptLanguage::_get_global_class_name(const String &path) const {
	Ref<LuaScript> script = ResourceLoader::get_singleton()->load(path);
	
	Dictionary result;
	if (script.is_valid() && script->_is_valid()) {
		result["name"] = script->_get_global_name();
		result["base_type"] = script->_get_instance_base_type();
		result["icon_path"] = script->_get_class_icon_path();
		result["is_abstract"] = script->_is_abstract();
		result["is_tool"] = script->_is_tool();
	}
	return result;
}

LuaState *LuaScriptLanguage::get_lua_state() {
	return lua_state.ptr();
}

LuaScriptLanguage *LuaScriptLanguage::get_singleton() {
	return instance;
}

LuaScriptLanguage *LuaScriptLanguage::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(LuaScriptLanguage);
		Engine::get_singleton()->register_script_language(instance);
	}
	return instance;
}

void LuaScriptLanguage::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_script_language(instance);
		memdelete(instance);
		instance = nullptr;
	}
}

void LuaScriptLanguage::_bind_methods() {
}

LuaScriptLanguage *LuaScriptLanguage::instance = nullptr;

}
