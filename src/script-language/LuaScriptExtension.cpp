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
#include "LuaScriptExtension.hpp"

#include "LuaScriptLanguageExtension.hpp"
#include "../LuaError.hpp"
#include "../LuaState.hpp"

#include "godot_cpp/classes/global_constants.hpp"

namespace luagdextension {

bool LuaScriptExtension::_has_source_code() const {
	return !source_code.is_empty();
}

String LuaScriptExtension::_get_source_code() const {
	return source_code;
}

void LuaScriptExtension::_set_source_code(const String &code) {
	source_code = code;
}

Error LuaScriptExtension::_reload(bool keep_state) {
	Variant result = LuaScriptLanguageExtension::get_singleton()->get_lua_state()->do_string(source_code);
	if (LuaError *error = Object::cast_to<LuaError>(result)) {
		loaded_class.clear();
		switch (error->get_status()) {
			case LuaError::MEMORY:
			case LuaError::GC:
				return ERR_OUT_OF_MEMORY;

			case LuaError::SYNTAX:
				return ERR_PARSE_ERROR;

			default:
				return ERR_SCRIPT_FAILED;
		}
	}
	else {
		loaded_class = result;
		return OK;
	}
}

bool LuaScriptExtension::_is_valid() const {
	return loaded_class;
}

ScriptLanguage *LuaScriptExtension::_get_language() const {
	return Object::cast_to<ScriptLanguage>(LuaScriptLanguageExtension::get_singleton());
}

void LuaScriptExtension::_bind_methods() {
}

}

