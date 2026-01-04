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
#ifndef __LUA_SCRIPT_METADATA_HPP__
#define __LUA_SCRIPT_METADATA_HPP__

#include <godot_cpp/templates/hash_map.hpp>

#include "LuaScriptMethod.hpp"
#include "LuaScriptProperty.hpp"
#include "LuaScriptSignal.hpp"

using namespace godot;

namespace luagdextension {

struct LuaScriptMetadata {
	bool is_valid;
	bool is_tool;
	StringName base_class;
	StringName class_name;
	String icon_path;
	Variant rpc_config;
	HashMap<StringName, LuaScriptMethod> methods;
	HashMap<StringName, LuaScriptProperty> properties;
	HashMap<StringName, LuaScriptSignal> signals;

	void setup(const sol::table& t);
	void clear();
};

}

#endif  // __LUA_SCRIPT_METADATA_HPP__
