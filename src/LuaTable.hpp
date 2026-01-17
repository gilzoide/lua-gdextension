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
#ifndef __LUA_TABLE_HPP__
#define __LUA_TABLE_HPP__

#include "LuaObject.hpp"

#include "utils/custom_sol.hpp"

#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

namespace luagdextension {

class LuaTable : public LuaObjectSubclass<sol::table> {
	GDCLASS(LuaTable, LuaObject);

public:
	LuaTable();
	LuaTable(sol::table&& table);
	LuaTable(const sol::table& table);

	sol::optional<Variant> try_get(const Variant& key, bool raw = false) const;
	bool try_set(const Variant& key, const Variant& value, bool raw = false);

	Variant get(const Variant& key, const Variant& default_value = Variant()) const;
	Variant rawget(const Variant& key, const Variant& default_value = Variant()) const;
	void set(const Variant& key, const Variant& value);
	void rawset(const Variant& key, const Variant& value);
	void clear();
	int64_t length() const;

	Dictionary to_dictionary() const;
	Array to_array() const;

	Ref<LuaTable> get_metatable() const;
	void set_metatable(LuaTable *metatable);

	bool _iter_init(const Variant& iter) const;
	bool _iter_next(const Variant& iter) const;
	Variant _iter_get(const Variant& iter) const;

	struct Iterator {
		Iterator() = default;
		Iterator(Ref<LuaTable> table);

		Variant operator*() const;
		Iterator& operator++();
		Iterator operator++(int);
		bool operator==(const Iterator& other);
		bool operator!=(const Iterator& other);
		
	private:
		Ref<LuaTable> table;
		Array iter;
	};

	Iterator begin();
	Iterator end();

	sol::table& get_table();
	const sol::table& get_table() const;

protected:
	static void _bind_methods();
	
	bool _get(const StringName& property_name, Variant& r_value) const;
	bool _set(const StringName& property_name, const Variant& value);

	String _to_string() const override;
};

}

#endif
