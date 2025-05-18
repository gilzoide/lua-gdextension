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
#include "LuaCodeEdit.hpp"
#include "LuaScriptLanguage.hpp"

namespace luagdextension {

bool LuaCodeEdit::_property_can_revert(const StringName &p_name) const {
	return Array::make("delimiter_comments", "delimiter_strings").has(p_name);
}

bool LuaCodeEdit::_property_get_revert(const StringName &p_name, Variant &r_property) const {
	if (p_name == String("delimiter_comments")) {
		r_property = LuaScriptLanguage::get_singleton()->_get_comment_delimiters();
		return true;
	}
	else if (p_name == String("delimiter_strings")) {
		r_property = LuaScriptLanguage::get_singleton()->_get_string_delimiters();
		return true;
	}
	else {
		return false;
	}
}

void LuaCodeEdit::_bind_methods() {}

}
