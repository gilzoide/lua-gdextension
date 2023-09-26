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
#include "utils/convert_godot_lua.hpp"
#include "utils/convert_godot_std.hpp"
#include "utils/metatable.hpp"

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
	return luagdextension::to_dictionary(table);
}

Array LuaTable::to_array() const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), Array(), "LuaTable does not have a valid table");
	return luagdextension::to_array(table);
}

bool LuaTable::_iter_init(const Variant& iter) const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), false, "LuaTable does not have a valid table");
	lua_State *L = table.lua_state();
	auto table_popper = sol::stack::push_pop(table);
	lua_pushnil(L);
	if (lua_next(L, -2)) {
		Array arg = iter;
		arg[0] = to_variant(sol::stack_object(L, -2));
		lua_pop(L, 2);
		return true;
	}
	else {
		return false;
	}
}

bool LuaTable::_iter_next(const Variant& iter) const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), false, "LuaTable does not have a valid table");
	lua_State *L = table.lua_state();
	Array arg = iter;
	auto table_popper = sol::stack::push_pop(table);
	auto key = to_lua(L, arg[0]);
	if (lua_next(L, -2)) {
		arg[0] = to_variant(sol::stack_object(L, -2));
		lua_pop(L, 2);
		return true;
	}
	else {
		return false;
	}
}

Variant LuaTable::_iter_get(const Variant& iter) const {
	ERR_FAIL_COND_V_EDMSG(!table.valid(), Variant(), "LuaTable does not have a valid table");
	return iter;
}

void LuaTable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("geti", "index"), &LuaTable::geti);
	ClassDB::bind_method(D_METHOD("seti", "index", "value"), &LuaTable::seti);
	ClassDB::bind_method(D_METHOD("size"), &LuaTable::size);

	ClassDB::bind_method(D_METHOD("to_dictionary"), &LuaTable::to_dictionary);
	ClassDB::bind_method(D_METHOD("to_array"), &LuaTable::to_array);

	ClassDB::bind_method(D_METHOD("_iter_init", "iter"), &LuaTable::_iter_init);
	ClassDB::bind_method(D_METHOD("_iter_next", "iter"), &LuaTable::_iter_next);
	ClassDB::bind_method(D_METHOD("_iter_get", "iter"), &LuaTable::_iter_get);
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
	auto tostring_result = call_metamethod(table, sol::meta_function::to_string);
	if (tostring_result.has_value()) {
		return to_variant(tostring_result.value());
	}
	else {
		return String("[LuaTable:0x%x]") % (int64_t) table.pointer();
	}
}

}
