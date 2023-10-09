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
#ifndef __LUA_OBJECT_HPP__
#define __LUA_OBJECT_HPP__

#include "LuaState.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

class LuaObject : public RefCounted {
	GDCLASS(LuaObject, RefCounted);

public:
	virtual const sol::reference& get_lua_object() const;
	lua_State *get_lua_state() const;

	uint64_t to_pointer() const;

protected:
	static void _bind_methods();

	virtual String _to_string() const;
};


template<class TReference>
class LuaObjectSubclass : public LuaObject {
public:
	LuaObjectSubclass() {
		throw "FIXME: LuaObjectSubclass should never be instanced with default constructor";
	}
	LuaObjectSubclass(TReference&& lua_object) : lua_object(lua_object) {
		if (!lua_object.valid()) {
			throw "FIXME: invalid reference to Lua object";
		}
		if (LuaState *state = LuaState::find_lua_state(get_lua_state())) {
			lua_state = Ref(state);
		}
	}
	LuaObjectSubclass(const TReference& lua_object) : lua_object(lua_object) {
		if (!lua_object.valid()) {
			throw "FIXME: invalid reference to Lua object";
		}
		if (LuaState *state = LuaState::find_lua_state(get_lua_state())) {
			lua_state = Ref(state);
		}
	}

	const sol::reference& get_lua_object() const {
		return lua_object;
	}

protected:
	TReference lua_object;

private:
	Ref<LuaState> lua_state;
};

}

#endif

