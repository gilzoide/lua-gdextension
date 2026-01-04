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
#ifndef __LUA_PARSER_HPP__
#define __LUA_PARSER_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

typedef struct TSParser TSParser;

namespace luagdextension {

class LuaAST;

class LuaParser : public RefCounted {
	GDCLASS(LuaParser, RefCounted);
public:
	LuaParser();
	virtual ~LuaParser();

	Ref<LuaAST> parse_code(const String& code) const;
	
	bool get_debug_log() const;
	void set_debug_log(bool enabled);

	static void setup_tree_sitter_allocator();

protected:
	static void _bind_methods();
	String _to_string() const;

	TSParser *parser;
};

}

#endif  // __LUA_PARSER_HPP__
