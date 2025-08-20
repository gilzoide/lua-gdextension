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
#ifndef __LUA_SCRIPT_IMPORT_BEHAVIOR_MANAGER_HPP__
#define __LUA_SCRIPT_IMPORT_BEHAVIOR_MANAGER_HPP__

#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

namespace luagdextension {

class LuaScriptImportBehaviorManager {
public:
	LuaScriptImportBehaviorManager();
	
	void set_script_import_behavior(const String& script_path, int behavior);
	int get_script_import_behavior(const String& script_path) const;
	
	static LuaScriptImportBehaviorManager *get_singleton();
	static LuaScriptImportBehaviorManager *get_or_create_singleton();
	static void delete_singleton();

private:
	static LuaScriptImportBehaviorManager *instance;
	
	Dictionary map;
};

}

#endif  // __LUA_SCRIPT_IMPORT_BEHAVIOR_MANAGER_HPP__
