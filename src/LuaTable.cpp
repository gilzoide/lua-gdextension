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

#include "LuaTable.hpp"

#include "LuaState.hpp"
#include "godot_utils.hpp"
#include "lua_utils.hpp"

using namespace godot;

namespace luagdextension {

LuaTable::LuaTable() : table() {}

LuaTable::LuaTable(sol::table&& table) : table(table) {}

LuaTable::LuaTable(const sol::table& table) : table(table) {}

LuaTable::~LuaTable() {
	if (table.valid() && LuaState::is_valid(table.lua_state())) {
		table.~basic_table_core();
	}
}

Variant LuaTable::geti(int64_t index) const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), Variant(), "LuaTable does not have a valid table");

	return to_variant(table[index].get<sol::object>());
}

void LuaTable::seti(int64_t index, const Variant& value) {
	table[index] = to_lua(table.lua_state(), value);
}

size_t LuaTable::size() const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), 0, "LuaTable does not have a valid table");

	return table.size();
}

Dictionary LuaTable::to_dictionary() const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), Dictionary(), "LuaTable does not have a valid table");
	
	Dictionary dict;
	for (auto it : table) {
		sol::object key, value;
		std::tie(key, value) = it;
		dict[to_variant(key)] = to_variant(value);
	}
	return dict;
}

Array LuaTable::to_array() const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), Array(), "LuaTable does not have a valid table");

	Array arr;
	for (int i = 1; i <= table.size(); i++) {
		arr.append(to_variant(table.get<sol::object>(i)));
	}
	return arr;
}

void LuaTable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("geti"), &LuaTable::geti);
	ClassDB::bind_method(D_METHOD("seti"), &LuaTable::seti);
	ClassDB::bind_method(D_METHOD("size"), &LuaTable::size);

	ClassDB::bind_method(D_METHOD("to_dictionary"), &LuaTable::to_dictionary);
	ClassDB::bind_method(D_METHOD("to_array"), &LuaTable::to_array);
}

bool LuaTable::_get(const StringName& property_name, Variant& r_value) const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), false, "LuaTable does not have a valid table");

	PackedByteArray bytes = property_name.to_utf8_buffer();
	r_value = to_variant(table[to_string_view(bytes)].get<sol::object>());

	return true;
}

bool LuaTable::_set(const StringName& property_name, const Variant& value) {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), false, "LuaTable does not have a valid table");

	PackedByteArray bytes = property_name.to_utf8_buffer();
	table[to_string_view(bytes)] = to_lua(table.lua_state(), value);

	return true;
}

String LuaTable::_to_string() const {
	return String("[LuaTable:0x%x]") % (int64_t) table.pointer();
}

}
