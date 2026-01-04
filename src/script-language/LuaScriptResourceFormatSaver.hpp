/**
 * Copyright (C) 2026 Gil Barbosa Reis.
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
#ifndef __LUA_SCRIPT_RESOURCE_FORMAT_SAVER_HPP__
#define __LUA_SCRIPT_RESOURCE_FORMAT_SAVER_HPP__

#include <godot_cpp/classes/resource_format_saver.hpp>

using namespace godot;

namespace luagdextension {

class LuaScriptResourceFormatSaver : public ResourceFormatSaver {
	GDCLASS(LuaScriptResourceFormatSaver, ResourceFormatSaver);
public:
	Error _save(const Ref<Resource> &resource, const String &path, uint32_t flags) override;
	// Error _set_uid(const String &path, int64_t uid) override;
	bool _recognize(const Ref<Resource> &resource) const override;
	PackedStringArray _get_recognized_extensions(const Ref<Resource> &resource) const override;
	// bool _recognize_path(const Ref<Resource> &resource, const String &path) const override;

	static void register_in_godot();
	static void unregister_in_godot();

protected:
	static void _bind_methods();
};

}


#endif  // __LUA_SCRIPT_RESOURCE_FORMAT_SAVER_HPP__
