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

#include "utils/convert_godot_lua.hpp"
#include "utils/convert_godot_std.hpp"
#include "utils/metatable.hpp"

#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

namespace luagdextension {

LuaTable::LuaTable() : LuaObjectSubclass() {}
LuaTable::LuaTable(sol::table&& table) : LuaObjectSubclass(table) {}
LuaTable::LuaTable(const sol::table& table) : LuaObjectSubclass(table) {}

Variant LuaTable::get_value(const Variant& key, const Variant& default_value) const {
	lua_State *L = get_lua_state();
	sol::stack::push(L, lua_object);
	std::ignore = to_lua(L, key);
	lua_gettable(L, -2);
	Variant value;
	if (lua_isnoneornil(L, -1)) {
		value = default_value;
	}
	else {
		value = to_variant(L, -1);
	}
	lua_pop(L, 2);
	return value;
}

void LuaTable::set_value(const Variant& key, const Variant& value) {
	lua_State *L = get_lua_state();
	sol::stack::push(L, lua_object);
	std::ignore = to_lua(L, key);
	std::ignore = to_lua(L, value);
	lua_settable(L, -3);
	lua_pop(L, 1);
}

int64_t LuaTable::length() const {
	return lua_object.size();
}

Dictionary LuaTable::to_dictionary() const {
	return luagdextension::to_dictionary(lua_object);
}

Array LuaTable::to_array() const {
	return luagdextension::to_array(lua_object);
}

bool LuaTable::_iter_init(const Variant& iter) const {
	lua_State *L = get_lua_state();
	auto table_popper = sol::stack::push_pop(lua_object);
	lua_pushnil(L);
	if (lua_next(L, -2)) {
		Array arg = iter;
		arg.resize(1);
		arg[0] = to_variant(L, -2);
		lua_pop(L, 2);
		return true;
	}
	else {
		Array arg = iter;
		arg.clear();
		return false;
	}
}

bool LuaTable::_iter_next(const Variant& iter) const {
	lua_State *L = get_lua_state();
	Array arg = iter;
	auto table_popper = sol::stack::push_pop(lua_object);
	auto key = to_lua(L, arg[0]);
	if (lua_next(L, -2)) {
		arg[0] = to_variant(L, -2);
		lua_pop(L, 2);
		return true;
	}
	else {
		return false;
	}
}

Variant LuaTable::_iter_get(const Variant& iter) const {
	return iter;
}

LuaTable::Iterator LuaTable::begin() {
	return Iterator(this);
}

LuaTable::Iterator LuaTable::end() {
	return Iterator();
}

void LuaTable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_value", "key", "default"), &LuaTable::get_value, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("set_value", "key", "value"), &LuaTable::set_value);
	ClassDB::bind_method(D_METHOD("get", "key", "default"), &LuaTable::get_value, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("set", "key", "value"), &LuaTable::set_value);
	ClassDB::bind_method(D_METHOD("length"), &LuaTable::length);

	ClassDB::bind_method(D_METHOD("to_dictionary"), &LuaTable::to_dictionary);
	ClassDB::bind_method(D_METHOD("to_array"), &LuaTable::to_array);

	ClassDB::bind_method(D_METHOD("_iter_init", "iter"), &LuaTable::_iter_init);
	ClassDB::bind_method(D_METHOD("_iter_next", "iter"), &LuaTable::_iter_next);
	ClassDB::bind_method(D_METHOD("_iter_get", "iter"), &LuaTable::_iter_get);
}

bool LuaTable::_get(const StringName& property_name, Variant& r_value) const {
	PackedByteArray bytes = property_name.to_utf8_buffer();
	r_value = to_variant(lua_object[to_string_view(bytes)].get<sol::object>());

	return true;
}

bool LuaTable::_set(const StringName& property_name, const Variant& value) {
	PackedByteArray bytes = property_name.to_utf8_buffer();
	lua_object[to_string_view(bytes)] = to_lua(get_lua_state(), value);

	return true;
}

String LuaTable::_to_string() const {
	auto tostring_result = call_metamethod(lua_object, sol::meta_function::to_string);
	if (tostring_result.has_value()) {
		return to_variant(tostring_result.value());
	}
	else {
		return LuaObjectSubclass::_to_string();
	}
}

// LuaTable::Iterator
LuaTable::Iterator::Iterator(Ref<LuaTable> table)
	: table(table)
	, iter()
{
	if (!table.is_valid()) {
		return;
	}
	if (!table->_iter_init(iter)) {
		iter.clear();
	}
}

Variant LuaTable::Iterator::operator*() const {
	return iter.is_empty() ? Variant() : iter[0];
}

LuaTable::Iterator& LuaTable::Iterator::operator++() {
	if (!table->_iter_next(iter)) {
		iter.clear();
	}
	return *this;
}

LuaTable::Iterator LuaTable::Iterator::operator++(int) {
	Iterator copy(*this);
	operator++();
	return copy;
}

bool LuaTable::Iterator::operator==(const LuaTable::Iterator& other) {
	return iter == other.iter;
}

bool LuaTable::Iterator::operator!=(const LuaTable::Iterator& other) {
	return iter != other.iter;
}

}
