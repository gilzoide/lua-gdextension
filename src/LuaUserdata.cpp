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

#include "LuaUserdata.hpp"

#include "LuaState.hpp"
#include "utils/convert_godot_lua.hpp"
#include "utils/metatable.hpp"

using namespace godot;

namespace luagdextension {

LuaUserdata::LuaUserdata() : LuaTable(true) {
	ERR_PRINT("LuaUserdata should never be instantiated manually!");
}

LuaUserdata::LuaUserdata(sol::userdata&& userdata) : LuaTable(userdata) {}

LuaUserdata::LuaUserdata(const sol::userdata& userdata) : LuaTable(userdata) {}

void LuaUserdata::_bind_methods() {
}

LuaUserdata::operator String() const {
	return _to_string();
}

String LuaUserdata::_to_string() const {
	auto tostring_result = call_metamethod(table, sol::meta_function::to_string);
	if (tostring_result.has_value()) {
		return to_variant(tostring_result.value());
	}
	else {
		return String("[LuaUserdata:0x%x]") % (int64_t) table.pointer();
	}
}

}
