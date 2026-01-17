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
#ifndef __LUA_AST_NODE_HPP__
#define __LUA_AST_NODE_HPP__

#include <godot_cpp/classes/ref_counted.hpp>
#include <tree_sitter/api.h>

using namespace godot;


namespace luagdextension {

class LuaAST;
class LuaASTQuery;

class LuaASTNode : public RefCounted {
	GDCLASS(LuaASTNode, RefCounted);
public:
	LuaASTNode();
	LuaASTNode(Ref<LuaAST> tree, TSNode node);

	String get_type() const;
	String get_grammar_type() const;
	uint32_t get_start_pos() const;
	uint32_t get_end_pos() const;
	Vector2i get_start_point() const;
	Vector2i get_end_point() const;
	String dump() const;
	bool is_named() const;
	bool is_missing() const;
	bool is_extra() const;
	bool has_errors() const;
	bool is_error() const;

	Ref<LuaASTNode> get_child(uint32_t index) const;
	uint32_t get_child_count() const;
	Ref<LuaASTNode> get_named_child(uint32_t index) const;
	uint32_t get_named_child_count() const;
	Ref<LuaASTNode> get_child_by_field_name(const String& field_name) const;
	
	Ref<LuaASTNode> get_parent() const;
	Ref<LuaASTNode> get_next_sibling() const;
	Ref<LuaASTNode> get_previous_sibling() const;
	Ref<LuaASTNode> get_next_named_sibling() const;
	Ref<LuaASTNode> get_previous_named_sibling() const;

	String get_source_code() const;

	Ref<LuaASTQuery> query(const String& query);

	Ref<LuaAST> get_tree() const;
	TSNode get_node() const;

	static Ref<LuaASTNode> to_object(Ref<LuaAST> tree, TSNode node);

protected:
	static void _bind_methods();
	String _to_string() const;

	Ref<LuaAST> tree;
	TSNode node;
};

}

#endif  // __LUA_AST_NODE_HPP__
