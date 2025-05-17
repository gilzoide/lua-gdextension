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
#include "LuaSyntaxHighlighter.hpp"
#include "LuaScriptLanguage.hpp"

namespace luagdextension {

Color LuaSyntaxHighlighter::get_lua_keyword_color() const {
	return lua_keyword_color;
}

void LuaSyntaxHighlighter::set_lua_keyword_color(Color color) {
	lua_keyword_color = color;
	Dictionary keyword_colors = get_keyword_colors();
	for (String& keyword : LuaScriptLanguage::get_singleton()->_get_reserved_words()) {
		keyword_colors[keyword] = color;
	}
	set_keyword_colors(keyword_colors);
	emit_changed();
}

Color LuaSyntaxHighlighter::get_string_color() const {
	return string_color;
}

void LuaSyntaxHighlighter::set_string_color(Color color) {
	string_color = color;
	Dictionary color_regions = get_color_regions();
	for (String& string_delimiter : LuaScriptLanguage::get_singleton()->_get_string_delimiters()) {
		color_regions[string_delimiter] = color;
	}
	set_color_regions(color_regions);
	emit_changed();
}

Color LuaSyntaxHighlighter::get_comment_color() const {
	return comment_color;
}

void LuaSyntaxHighlighter::set_comment_color(Color color) {
	comment_color = color;
	Dictionary color_regions = get_color_regions();
	for (String& comment_delimiter : LuaScriptLanguage::get_singleton()->_get_comment_delimiters()) {
		color_regions[comment_delimiter] = color;
	}
	set_color_regions(color_regions);
	emit_changed();
}

void LuaSyntaxHighlighter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_lua_keyword_color"), &LuaSyntaxHighlighter::get_lua_keyword_color);
	ClassDB::bind_method(D_METHOD("set_lua_keyword_color", "color"), &LuaSyntaxHighlighter::set_lua_keyword_color);
	ClassDB::bind_method(D_METHOD("get_string_color"), &LuaSyntaxHighlighter::get_string_color);
	ClassDB::bind_method(D_METHOD("set_string_color", "color"), &LuaSyntaxHighlighter::set_string_color);
	ClassDB::bind_method(D_METHOD("get_comment_color"), &LuaSyntaxHighlighter::get_comment_color);
	ClassDB::bind_method(D_METHOD("set_comment_color", "color"), &LuaSyntaxHighlighter::set_comment_color);

	ADD_PROPERTY(PropertyInfo(Variant::Type::COLOR, "lua_keyword_color"), "set_lua_keyword_color", "get_lua_keyword_color");
	ADD_PROPERTY(PropertyInfo(Variant::Type::COLOR, "string_color"), "set_string_color", "get_string_color");
	ADD_PROPERTY(PropertyInfo(Variant::Type::COLOR, "comment_color"), "set_comment_color", "get_comment_color");
}

}
