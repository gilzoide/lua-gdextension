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
#ifndef __LUA_AST_QUERY_HPP__
#define __LUA_AST_QUERY_HPP__

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <tree_sitter/api.h>

using namespace godot;


namespace luagdextension {

class LuaASTNode;

class LuaASTQuery : public RefCounted {
	GDCLASS(LuaASTQuery, RefCounted);
public:
	LuaASTQuery();
	virtual ~LuaASTQuery();

	bool is_valid() const;
	void set_query(const String& query);
	void set_node(Ref<LuaASTNode> node);

	Variant first_match();
	TypedArray<Array> all_matches();

	bool _iter_init(const Variant& iter) const;
	bool _iter_next(const Variant& iter) const;
	Variant _iter_get(const Variant& iter) const;

protected:
	static void _bind_methods();
	String _to_string() const;

	TSQueryCursor *cursor;
	TSQuery *query;
	Ref<LuaASTNode> node;
};

}

#endif  // __LUA_AST_QUERY_HPP__
