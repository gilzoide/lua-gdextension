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
#include <string_view>

#include "LuaState.hpp"
#include "lua_utils.hpp"

#include <godot_cpp/core/binder_common.hpp>

namespace luagdextension {

LuaState::LuaState() : lua_state(sol::default_at_panic, lua_alloc) {}

void LuaState::_bind_methods() {
	// Library enum
	BIND_BITFIELD_FLAG(BASE);
	BIND_BITFIELD_FLAG(PACKAGE);
	BIND_BITFIELD_FLAG(COROUTINE);
	BIND_BITFIELD_FLAG(STRING);
	BIND_BITFIELD_FLAG(OS);
	BIND_BITFIELD_FLAG(MATH);
	BIND_BITFIELD_FLAG(TABLE);
	BIND_BITFIELD_FLAG(DEBUG);
	BIND_BITFIELD_FLAG(BIT32);
	BIND_BITFIELD_FLAG(IO);
	BIND_BITFIELD_FLAG(FFI);
	BIND_BITFIELD_FLAG(JIT);
	BIND_BITFIELD_FLAG(UTF8);
	BIND_BITFIELD_FLAG(LUA);

	// Methods
	ClassDB::bind_method(D_METHOD("open_libraries", "libraries"), &LuaState::open_libraries, DEFVAL(BitField<Library>(LUA)));
	ClassDB::bind_method(D_METHOD("do_string", "chunk"), &LuaState::do_string);
}

void LuaState::open_libraries(BitField<Library> libraries) {
	if (libraries == 0) {
		libraries = LUA;
	}

	if (libraries.has_flag(LUA)) {
		lua_state.open_libraries();
	}
	else {
		if (libraries.has_flag(BASE)) {
			lua_state.open_libraries(sol::lib::base);
		}
		if (libraries.has_flag(PACKAGE)) {
			lua_state.open_libraries(sol::lib::package);
		}
		if (libraries.has_flag(COROUTINE)) {
			lua_state.open_libraries(sol::lib::coroutine);
		}
		if (libraries.has_flag(STRING)) {
			lua_state.open_libraries(sol::lib::string);
		}
		if (libraries.has_flag(OS)) {
			lua_state.open_libraries(sol::lib::os);
		}
		if (libraries.has_flag(MATH)) {
			lua_state.open_libraries(sol::lib::math);
		}
		if (libraries.has_flag(TABLE)) {
			lua_state.open_libraries(sol::lib::table);
		}
		if (libraries.has_flag(DEBUG)) {
			lua_state.open_libraries(sol::lib::debug);
		}
		if (libraries.has_flag(BIT32)) {
			lua_state.open_libraries(sol::lib::bit32);
		}
		if (libraries.has_flag(IO)) {
			lua_state.open_libraries(sol::lib::io);
		}
		if (libraries.has_flag(FFI)) {
			lua_state.open_libraries(sol::lib::ffi);
		}
		if (libraries.has_flag(JIT)) {
			lua_state.open_libraries(sol::lib::jit);
		}
		if (libraries.has_flag(UTF8)) {
			lua_state.open_libraries(sol::lib::utf8);
		}
	}
}

Variant LuaState::do_string(String chunk) {
	CharString chunk_utf8 = chunk.utf8();
	std::string_view lua_code_view(chunk_utf8.get_data(), chunk_utf8.length());
	return to_variant(lua_state.safe_script(lua_code_view, sol::script_pass_on_error));
}

}
