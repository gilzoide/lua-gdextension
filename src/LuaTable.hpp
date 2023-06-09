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
#ifndef __LUA_TABLE_HPP__
#define __LUA_TABLE_HPP__

#include "custom_sol.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

namespace luagdextension {

class LuaTable : public RefCounted {
	GDCLASS(LuaTable, RefCounted);

public:
	LuaTable();
	LuaTable(sol::table&& table);
	LuaTable(const sol::table& table);
	~LuaTable();

	Variant geti(int64_t index) const;
	void seti(int64_t index, const Variant& value);
	size_t size() const;

	Dictionary to_dictionary() const;
	Array to_array() const;

protected:
	static void _bind_methods();
	
	bool _get(const StringName& property_name, Variant& r_value) const;
	bool _set(const StringName& property_name, const Variant& value);
	String _to_string() const;

	// Using union avoids automatic destruction
	// This is necessary to only destroy tables if the corresponding LuaState is still valid
	union {
		sol::table table;
	};
};

}

#endif
