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
#ifndef __LUA_SCRIPT_SYNTAX_HIGHLIGHTER_HPP__
#define __LUA_SCRIPT_SYNTAX_HIGHLIGHTER_HPP__

#include <godot_cpp/classes/code_highlighter.hpp>

using namespace godot;

namespace luagdextension {

class LuaSyntaxHighlighter : public CodeHighlighter {
	GDCLASS(LuaSyntaxHighlighter, CodeHighlighter)

public:
	Color get_lua_keyword_color() const;
	void set_lua_keyword_color(Color color);

	Color get_string_color() const;
	void set_string_color(Color color);
	
	Color get_comment_color() const;
	void set_comment_color(Color color);

protected:
	static void _bind_methods();

	Color lua_keyword_color;
	Color string_color;
	Color comment_color;
};

}

#endif
