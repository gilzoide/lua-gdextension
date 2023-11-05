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
#include "LuaScriptLanguageExtension.hpp"

#include "LuaScriptExtension.hpp"
#include "../LuaState.hpp"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace luagdextension {

String LuaScriptLanguageExtension::_get_name() const {
	return "Lua";
}

void LuaScriptLanguageExtension::_init() {
	lua_state = memnew(LuaState);
	lua_state->open_libraries();
}

String LuaScriptLanguageExtension::_get_type() const {
	return "Lua";
}

String LuaScriptLanguageExtension::_get_extension() const {
	return "lua";
}

void LuaScriptLanguageExtension::_finish() {
	memdelete(lua_state);
}

PackedStringArray LuaScriptLanguageExtension::_get_reserved_words() const {
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

bool LuaScriptLanguageExtension::_is_control_flow_keyword(const String &keyword) const {
	return godot::helpers::append_all(PackedStringArray(),
		"break", "do", "else", "elseif", "end",
		"for", "goto", "if", "in",
		"repeat", "return",
		"then", "until", "while"
	).has(keyword);
}

PackedStringArray LuaScriptLanguageExtension::_get_comment_delimiters() const {
	return godot::helpers::append_all(PackedStringArray(),
		"--", "--[[ ]]"
	);
}

PackedStringArray LuaScriptLanguageExtension::_get_string_delimiters() const {
	return godot::helpers::append_all(PackedStringArray(),
		"' '", "\" \"", "[[ ]]", "[=[ ]=]"
	);
}

Ref<Script> LuaScriptLanguageExtension::_make_template(const String &_template, const String &class_name, const String &base_class_name) const {
	Ref<LuaScriptExtension> script = memnew(LuaScriptExtension);
	String source_code = _template.replace("_BASE_", base_class_name)
		.replace("_CLASS_", class_name)
		.replace("_TS_", "\t");
	script->set_source_code(source_code);
	return script;
}

bool LuaScriptLanguageExtension::_is_using_templates() {
	return false;
}

String LuaScriptLanguageExtension::_validate_path(const String &path) const {
	return "";
}

bool LuaScriptLanguageExtension::_has_named_classes() const {
	return false;
}

bool LuaScriptLanguageExtension::_supports_builtin_mode() const {
	return false;
}

bool LuaScriptLanguageExtension::_supports_documentation() const {
	return false;
}

bool LuaScriptLanguageExtension::_can_inherit_from_file() const {
	return false;
}

bool LuaScriptLanguageExtension::_overrides_external_editor() {
	return false;
}

PackedStringArray LuaScriptLanguageExtension::_get_recognized_extensions() const {
	return godot::helpers::append_all(PackedStringArray(),
		"lua"
	);
}

void LuaScriptLanguageExtension::_frame() {
}

LuaScriptLanguageExtension *LuaScriptLanguageExtension::get_singleton() {
	return instance;
}

LuaScriptLanguageExtension *LuaScriptLanguageExtension::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(LuaScriptLanguageExtension);
		Engine::get_singleton()->register_script_language(instance);
	}
	return instance;
}

void LuaScriptLanguageExtension::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_script_language(instance);
		memdelete(instance);
		instance = nullptr;
	}
}

void LuaScriptLanguageExtension::_bind_methods() {
}

LuaScriptLanguageExtension *LuaScriptLanguageExtension::instance = nullptr;

}
