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
#include "LuaThread.hpp"

namespace luagdextension {

LuaThread::LuaThread() : LuaObjectSubclass() {}
LuaThread::LuaThread(sol::thread&& thread) : LuaObjectSubclass(thread) {}
LuaThread::LuaThread(const sol::thread& thread) : LuaObjectSubclass(thread) {}

LuaThread::Status LuaThread::get_status() const {
	return static_cast<Status>(lua_object.status());
}

void LuaThread::_bind_methods() {
	BIND_ENUM_CONSTANT(STATUS_OK);
	BIND_ENUM_CONSTANT(STATUS_YIELD);
	BIND_ENUM_CONSTANT(STATUS_ERRRUN);
	BIND_ENUM_CONSTANT(STATUS_ERRSYNTAX);
	BIND_ENUM_CONSTANT(STATUS_ERRMEM);
	BIND_ENUM_CONSTANT(STATUS_ERRERR);
	BIND_ENUM_CONSTANT(STATUS_DEAD);

	ClassDB::bind_method(D_METHOD("get_status"), &LuaThread::get_status);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "status", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CLASS_IS_ENUM, "Status"), "", "get_status");
}

}
