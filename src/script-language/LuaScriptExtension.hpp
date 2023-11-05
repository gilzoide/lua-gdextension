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
#ifndef __LUA_SCRIPT_EXTENSION_HPP__
#define __LUA_SCRIPT_EXTENSION_HPP__

#include <godot_cpp/classes/script_extension.hpp>

using namespace godot;

namespace luagdextension {

class LuaScriptLanguageExtension;

class LuaScriptExtension : public ScriptExtension {
	GDCLASS(LuaScriptExtension, ScriptExtension);
public:
	/* bool _editor_can_reload_from_file() override; */
	/* void _placeholder_erased(void *placeholder) override; */
	/* bool _can_instantiate() const override; */
	/* Ref<Script> _get_base_script() const override; */
	/* StringName _get_global_name() const override; */
	/* bool _inherits_script(const Ref<Script> &script) const override; */
	/* StringName _get_instance_base_type() const override; */
	/* void *_instance_create(Object *for_object) const override; */
	/* void *_placeholder_instance_create(Object *for_object) const override; */
	/* bool _instance_has(Object *object) const override; */
	bool _has_source_code() const override;
	String _get_source_code() const override;
	void _set_source_code(const String &code) override;
	/* Error _reload(bool keep_state) override; */
	/* TypedArray<Dictionary> _get_documentation() const override; */
	/* bool _has_method(const StringName &method) const override; */
	/* Dictionary _get_method_info(const StringName &method) const override; */
	/* bool _is_tool() const override; */
	/* bool _is_valid() const override; */
	ScriptLanguage *_get_language() const override;
	/* bool _has_script_signal(const StringName &signal) const override; */
	/* TypedArray<Dictionary> _get_script_signal_list() const override; */
	/* bool _has_property_default_value(const StringName &property) const override; */
	/* Variant _get_property_default_value(const StringName &property) const override; */
	/* void _update_exports() override; */
	/* TypedArray<Dictionary> _get_script_method_list() const override; */
	/* TypedArray<Dictionary> _get_script_property_list() const override; */
	/* int32_t _get_member_line(const StringName &member) const override; */
	/* Dictionary _get_constants() const override; */
	/* TypedArray<StringName> _get_members() const override; */
	/* bool _is_placeholder_fallback_enabled() const override; */
	/* Variant _get_rpc_config() const override; */

protected:
	static void _bind_methods();

	String source_code;
};

}

#endif  // __LUA_SCRIPT_LANGUAGE_EXTENSION_HPP__

