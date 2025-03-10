/**
 * Copyright (C) 2025 Gil Barbosa Reis.
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
#ifndef __LUA_SCRIPT_RESOURCE_FORMAT_LOADER_HPP__
#define __LUA_SCRIPT_RESOURCE_FORMAT_LOADER_HPP__

#include <godot_cpp/classes/resource_format_loader.hpp>

using namespace godot;

namespace luagdextension {

class LuaScriptResourceFormatLoader : public ResourceFormatLoader {
	GDCLASS(LuaScriptResourceFormatLoader, ResourceFormatLoader);
public:
	PackedStringArray _get_recognized_extensions() const override;
	// bool _recognize_path(const String &p_path, const StringName &p_type) const override;
	bool _handles_type(const StringName &p_type) const override;
	String _get_resource_type(const String &p_path) const override;
	// String _get_resource_script_class(const String &p_path) const override;
	// int64_t _get_resource_uid(const String &p_path) const override;
	// PackedStringArray _get_dependencies(const String &p_path, bool p_add_types) const override;
	// Error _rename_dependencies(const String &p_path, const Dictionary &p_renames) const override;
	bool _exists(const String &p_path) const override;
	// PackedStringArray _get_classes_used(const String &p_path) const override;
	Variant _load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override;

	static void register_in_godot();
	static void unregister_in_godot();

protected:
	static void _bind_methods();
};

}


#endif  // __LUA_SCRIPT_RESOURCE_FORMAT_LOADER_HPP__
