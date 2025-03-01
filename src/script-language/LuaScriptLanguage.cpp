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
#include "../LuaError.hpp"
#include "../LuaTable.hpp"
#include "../LuaState.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace luagdextension {

String LuaScriptLanguage::_get_name() const {
	return "Lua";
}

void LuaScriptLanguage::_init() {
	lua_state.instantiate();
	lua_state->open_libraries();
}

String LuaScriptLanguage::_get_type() const {
	return "Lua";
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

bool LuaScriptLanguage::_is_using_templates() {
	return false;
}

Dictionary LuaScriptLanguage::_validate(const String &script, const String &path, bool validate_functions, bool validate_errors, bool validate_warnings, bool validate_safe_lines) const {
	Dictionary result;
	Variant f = LuaScriptLanguage::get_singleton()->get_lua_state()->load_string(script, path);
	if (LuaError *error = Object::cast_to<LuaError>(f)) {
		Dictionary error_dict;
		error_dict["path"] = path;
		error_dict["line"] = 1;
		error_dict["column"] = 1;
		error_dict["message"] = error->get_message();
		result["errors"] = Array::make(error_dict);
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
	return false;
}

bool LuaScriptLanguage::_can_inherit_from_file() const {
	return false;
}

bool LuaScriptLanguage::_overrides_external_editor() {
	return false;
}

void LuaScriptLanguage::_thread_enter() {
}

void LuaScriptLanguage::_thread_exit() {
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

void LuaScriptLanguage::_frame() {
}

Dictionary LuaScriptLanguage::_get_global_class_name(const String &path) const {
	// TODO
	return Dictionary();
}

bool LuaScriptLanguage::_handles_global_class_type(const String &type) const {
	return lua_state->get_globals()->get_value(type);
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
