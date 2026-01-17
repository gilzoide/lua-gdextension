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
#include "LuaError.hpp"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace luagdextension {

LuaError::LuaError(Status status, const String& message) : status(status), message(message) {}
LuaError::LuaError(const sol::load_result& load_result) : status((LuaError::Status) load_result.status()), message(extract_message(load_result)) {}
LuaError::LuaError(const sol::protected_function_result& function_result) : status((LuaError::Status) function_result.status()), message(extract_message(function_result)) {}

void LuaError::_bind_methods() {
	// enum Status
	BIND_ENUM_CONSTANT(OK);
	BIND_ENUM_CONSTANT(YIELDED);
	BIND_ENUM_CONSTANT(RUNTIME);
	BIND_ENUM_CONSTANT(MEMORY);
	BIND_ENUM_CONSTANT(HANDLER);
	BIND_ENUM_CONSTANT(GC);
	BIND_ENUM_CONSTANT(SYNTAX);
	BIND_ENUM_CONSTANT(FILE);

	ClassDB::bind_method(D_METHOD("get_message"), &LuaError::get_message);
	ClassDB::bind_method(D_METHOD("set_message", "message"), &LuaError::set_message);
	ClassDB::bind_method(D_METHOD("get_status"), &LuaError::get_status);
	ClassDB::bind_method(D_METHOD("set_status", "status"), &LuaError::set_status);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "message"), "set_message", "get_message");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "status", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CLASS_IS_ENUM, "Status"), "set_status", "get_status");
}

String LuaError::_to_string() const {
	return message;
}

const String& LuaError::get_message() const {
	return message;
}

void LuaError::set_message(const String& message) {
	this->message = message;
}

LuaError::Status LuaError::get_status() const {
	return status;
}

void LuaError::set_status(Status status) {
	this->status = status;
}

String LuaError::extract_message(const sol::load_result& load_result) {
	return luaL_tolstring(load_result.lua_state(), load_result.stack_index(), NULL);
}

String LuaError::extract_message(const sol::protected_function_result& function_result) {
	return luaL_tolstring(function_result.lua_state(), function_result.stack_index() + function_result.return_count() - 1, NULL);
}

}
