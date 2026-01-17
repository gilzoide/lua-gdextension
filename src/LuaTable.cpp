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

#include "LuaTable.hpp"

#include "utils/convert_godot_lua.hpp"
#include "utils/metatable.hpp"
#include "utils/stack_top_checker.hpp"

#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

namespace luagdextension {

LuaTable::LuaTable() : LuaObjectSubclass() {}
LuaTable::LuaTable(sol::table&& table) : LuaObjectSubclass(table) {}
LuaTable::LuaTable(const sol::table& table) : LuaObjectSubclass(table) {}

sol::optional<Variant> LuaTable::try_get(const Variant& key, bool raw) const {
	StackTopChecker topcheck(lua_object.lua_state());
	lua_State *L = lua_object.lua_state();
	sol::stack::push(L, lua_object);
	lua_push(L, key);
	if (raw) {
		lua_rawget(L, -2);
	}
	else {
		lua_gettable(L, -2);
	}
	auto _ = sol::stack::pop_n(L, 2);
	if (lua_isnoneornil(L, -1)) {
		return std::nullopt;
	}
	else {
		return to_variant(L, -1);
	}
}

bool LuaTable::try_set(const Variant& key, const Variant& value, bool raw) {
	ERR_FAIL_COND_V_MSG(key == Variant(), false, "Table key cannot be null");
	
	StackTopChecker topcheck(lua_object.lua_state());
	lua_State *L = lua_object.lua_state();
	auto table_popper = sol::stack::push_pop(L, lua_object);
	lua_push(L, key);
	lua_push(L, value);
	if (raw) {
		lua_rawset(L, -3);
	}
	else {
		lua_settable(L, -3);
	}
	return true;
}

Variant LuaTable::get(const Variant& key, const Variant& default_value) const {
	return try_get(key).value_or(default_value);
}

Variant LuaTable::rawget(const Variant& key, const Variant& default_value) const {
	return try_get(key, true).value_or(default_value);
}

void LuaTable::set(const Variant& key, const Variant& value) {
	try_set(key, value);
}

void LuaTable::rawset(const Variant& key, const Variant& value) {
	try_set(key, value, true);
}

void LuaTable::clear() {
	StackTopChecker topcheck(lua_object.lua_state());
	for (auto [key, _] : lua_object) {
		lua_object[key] = sol::nil;
	}
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

Ref<LuaTable> LuaTable::get_metatable() const {
	if (sol::optional<sol::table> metatable = lua_object[sol::metatable_key]) {
		return LuaObject::wrap_object<LuaTable>(*metatable);
	}
	else {
		return nullptr;
	}
}

void LuaTable::set_metatable(LuaTable *metatable) {
	lua_object[sol::metatable_key] = metatable ? metatable->get_table() : sol::nil;
}

bool LuaTable::_iter_init(const Variant& iter) const {
	StackTopChecker topcheck(lua_object.lua_state());
	lua_State *L = lua_object.lua_state();
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
		return false;
	}
}

bool LuaTable::_iter_next(const Variant& iter) const {
	StackTopChecker topcheck(lua_object.lua_state());
	lua_State *L = lua_object.lua_state();
	Array arg = iter;
	auto table_popper = sol::stack::push_pop(lua_object);
	lua_push(L, arg[0]);
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

sol::table& LuaTable::get_table() {
	return lua_object;
}

const sol::table& LuaTable::get_table() const {
	return lua_object;
}

void LuaTable::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get", "key", "default"), &LuaTable::get, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("set", "key", "value"), &LuaTable::set);
	ClassDB::bind_method(D_METHOD("rawget", "key", "default"), &LuaTable::rawget, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("rawset", "key", "value"), &LuaTable::rawset);
	ClassDB::bind_method(D_METHOD("clear"), &LuaTable::clear);
	ClassDB::bind_method(D_METHOD("length"), &LuaTable::length);

	ClassDB::bind_method(D_METHOD("to_dictionary"), &LuaTable::to_dictionary);
	ClassDB::bind_method(D_METHOD("to_array"), &LuaTable::to_array);

	ClassDB::bind_method(D_METHOD("get_metatable"), &LuaTable::get_metatable);
	ClassDB::bind_method(D_METHOD("set_metatable", "metatable"), &LuaTable::set_metatable);

	ClassDB::bind_method(D_METHOD("_iter_init", "iter"), &LuaTable::_iter_init);
	ClassDB::bind_method(D_METHOD("_iter_next", "iter"), &LuaTable::_iter_next);
	ClassDB::bind_method(D_METHOD("_iter_get", "iter"), &LuaTable::_iter_get);
}

bool LuaTable::_get(const StringName& property_name, Variant& r_value) const {
	r_value = get(property_name);
	return true;
}

bool LuaTable::_set(const StringName& property_name, const Variant& value) {
	set(property_name, value);
	return true;
}

String LuaTable::_to_string() const {
	auto tostring_result = call_metamethod(lua_object, sol::meta_function::to_string);
	if (tostring_result.has_value()) {
		return to_variant(tostring_result.value(), false);
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
