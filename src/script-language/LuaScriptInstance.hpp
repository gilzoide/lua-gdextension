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
#ifndef __LUA_SCRIPT_INSTANCE_HPP__
#define __LUA_SCRIPT_INSTANCE_HPP__

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include "../utils/custom_sol.hpp"

using namespace godot;

namespace luagdextension {

class LuaScript;
class LuaState;
class LuaTable;

struct LuaScriptInstance {
	LuaScriptInstance(Object *owner, Ref<LuaScript> script);
	~LuaScriptInstance();

	static GDExtensionScriptInstanceInfo3 *get_script_instance_info();
	static LuaScriptInstance *attached_to_object(Object *owner);

	template<typename ref_t>
	static LuaScriptInstance *find_instance(sol::basic_table<ref_t> t) {
		if (LuaScriptInstance **instance_ptr = table_to_instance.getptr(t.pointer())) {
			return *instance_ptr;
		}
		else {
			return nullptr;
		}
	}

	Object *owner;
	Ref<LuaScript> script;
	const void* data_ptr;
	
	sol::table get_data() const;

	void ensure_strong_ref();
	void ensure_weak_ref();

	static void register_lua(lua_State *L);
	static void unregister_lua(lua_State *L);
	
	static sol::protected_function rawget;
	static sol::protected_function rawset;

private:
	static HashMap<Object *, LuaScriptInstance *> owner_to_instance;
	static HashMap<const void *, LuaScriptInstance *> table_to_instance;

	// Data table's metatable
	static sol::table metatable;

	// These tables map LuaScriptInstance pointers to their data table
	// strong_refs is used when there are unmanaged references to the owner, to avoid Lua GCing the object.
	static sol::table strong_refs;
	// weak_refs is used when there are no unmanaged references to the owner, so Lua can GC the object.
	static sol::table weak_refs;
};

}

#endif  // __LUA_SCRIPT_INSTANCE_HPP__
