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
#ifndef __LUA_SCRIPT_INSTANCE_HPP__
#define __LUA_SCRIPT_INSTANCE_HPP__

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace luagdextension {

class LuaScript;
class LuaTable;

struct LuaScriptInstance {
	LuaScriptInstance(Object *owner, Ref<LuaScript> script);
	~LuaScriptInstance();

	static GDExtensionScriptInstanceInfo3 *get_script_instance_info();
	static LuaScriptInstance *attached_to_object(Object *owner);

	Object *owner;
	Ref<LuaScript> script;
	Ref<LuaTable> data;

	static Variant rawget(const Variant& self, const Variant& index);
	static void rawset(const Variant& self, const Variant& index, const Variant& value);

private:
	static HashMap<Object *, LuaScriptInstance *> known_instances;
};

}

#endif  // __LUA_SCRIPT_INSTANCE_HPP__
