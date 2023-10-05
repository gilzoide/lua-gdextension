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
#include "LuaLightUserdata.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

LuaLightUserdata::LuaLightUserdata() : lightuserdata() {}
LuaLightUserdata::LuaLightUserdata(sol::lightuserdata&& lightuserdata) : lightuserdata(lightuserdata) {}
LuaLightUserdata::LuaLightUserdata(const sol::lightuserdata& lightuserdata) : lightuserdata(lightuserdata) {}

uint64_t LuaLightUserdata::get_value() const {
	return (uint64_t) lightuserdata.as<void *>();
}

LuaLightUserdata::operator String() const {
	return _to_string();
}

void LuaLightUserdata::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_value"), &LuaLightUserdata::get_value);
}

String LuaLightUserdata::_to_string() const {
	return UtilityFunctions::str("[LuaLightUserdata:0x", String::num_uint64(get_value(), 16), "]");
}

}
