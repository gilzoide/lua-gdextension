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
#include "LuaObject.hpp"

namespace luagdextension {

const sol::reference& LuaObject::get_lua_object() const {
	throw "LuaObject subclass must override get_lua_object!";
}

lua_State *LuaObject::get_lua_state() const {
	return get_lua_object().lua_state();
}

uint64_t LuaObject::to_pointer() const {
	return (int64_t) get_lua_object().pointer();
}

void LuaObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("to_pointer"), &LuaObject::to_pointer);
}

String LuaObject::_to_string() const {
	return String("[%s:0x%x]") % Array::make(get_class(), to_pointer());
}

}
