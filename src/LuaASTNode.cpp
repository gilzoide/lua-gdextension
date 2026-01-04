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
#include "LuaASTNode.hpp"

#include "LuaAST.hpp"
#include "LuaASTQuery.hpp"

#include <tree_sitter/api.h>

using namespace godot;

namespace luagdextension {

LuaASTNode::LuaASTNode() {
	ERR_FAIL_MSG("FIXME: LuaASTNode should never be instanced with default constructor");
}
LuaASTNode::LuaASTNode(Ref<LuaAST> tree, TSNode node)
	: tree(tree)
	, node(node)
{
}

String LuaASTNode::get_type() const {
	return ts_node_type(node);
}

String LuaASTNode::get_grammar_type() const {
	return ts_node_grammar_type(node);
}

uint32_t LuaASTNode::get_start_pos() const {
	return ts_node_start_byte(node) / sizeof(char32_t);
}

uint32_t LuaASTNode::get_end_pos() const {
	return ts_node_end_byte(node) / sizeof(char32_t);
}

Vector2i LuaASTNode::get_start_point() const {
	TSPoint point = ts_node_start_point(node);
	return Vector2i(point.column / sizeof(char32_t), point.row);
}

Vector2i LuaASTNode::get_end_point() const {
	TSPoint point = ts_node_end_point(node);
	return Vector2i(point.column / sizeof(char32_t), point.row);
}

String LuaASTNode::dump() const {
	if (char *str = ts_node_string(node)) {
		String s(str);
		memfree(str);
		return s;
	}
	else {
		return "";
	}
}

bool LuaASTNode::is_named() const {
	return ts_node_is_named(node);
}

bool LuaASTNode::is_missing() const {
	return ts_node_is_missing(node);
}

bool LuaASTNode::is_extra() const {
	return ts_node_is_extra(node);
}

bool LuaASTNode::has_errors() const {
	return ts_node_has_error(node);
}

bool LuaASTNode::is_error() const {
	return ts_node_is_error(node);
}

Ref<LuaASTNode> LuaASTNode::get_child(uint32_t index) const {
	return to_object(tree, ts_node_child(node, index));
}

uint32_t LuaASTNode::get_child_count() const {
	return ts_node_child_count(node);
}

Ref<LuaASTNode> LuaASTNode::get_named_child(uint32_t index) const {
	return to_object(tree, ts_node_named_child(node, index));
}

uint32_t LuaASTNode::get_named_child_count() const {
	return ts_node_named_child_count(node);
}

Ref<LuaASTNode> LuaASTNode::get_child_by_field_name(const String& field_name) const {
	CharString field_name_chars = field_name.utf8();
	return to_object(tree, ts_node_child_by_field_name(node, field_name_chars.ptr(), field_name_chars.length()));
}

Ref<LuaASTNode> LuaASTNode::get_parent() const {
	return to_object(tree, ts_node_parent(node));
}

Ref<LuaASTNode> LuaASTNode::get_next_sibling() const {
	return to_object(tree, ts_node_next_sibling(node));
}

Ref<LuaASTNode> LuaASTNode::get_previous_sibling() const {
	return to_object(tree, ts_node_prev_sibling(node));
}

Ref<LuaASTNode> LuaASTNode::get_next_named_sibling() const {
	return to_object(tree, ts_node_next_named_sibling(node));
}

Ref<LuaASTNode> LuaASTNode::get_previous_named_sibling() const {
	return to_object(tree, ts_node_prev_named_sibling(node));
}

String LuaASTNode::get_source_code() const {
	return tree->get_source_code().substr(get_start_pos(), get_end_pos() - get_start_pos());
}

Ref<LuaASTQuery> LuaASTNode::query(const String& query) {
	Ref<LuaASTQuery> astQuery;
	astQuery.instantiate();
	astQuery->set_query(query);
	astQuery->set_node(this);
	return astQuery;
}

Ref<LuaAST> LuaASTNode::get_tree() const {
	return tree;
}

TSNode LuaASTNode::get_node() const {
	return node;
}

Ref<LuaASTNode> LuaASTNode::to_object(Ref<LuaAST> tree, TSNode node) {
	if (ts_node_is_null(node)) {
		return nullptr;
	}
	else {
		return memnew(LuaASTNode(tree, node));
	}
}

void LuaASTNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &LuaASTNode::get_type);
	ClassDB::bind_method(D_METHOD("get_grammar_type"), &LuaASTNode::get_grammar_type);
	ClassDB::bind_method(D_METHOD("get_start_pos"), &LuaASTNode::get_start_pos);
	ClassDB::bind_method(D_METHOD("get_end_pos"), &LuaASTNode::get_end_pos);
	ClassDB::bind_method(D_METHOD("get_start_point"), &LuaASTNode::get_start_point);
	ClassDB::bind_method(D_METHOD("get_end_point"), &LuaASTNode::get_end_point);
	ClassDB::bind_method(D_METHOD("dump"), &LuaASTNode::dump);
	ClassDB::bind_method(D_METHOD("is_named"), &LuaASTNode::is_named);
	ClassDB::bind_method(D_METHOD("is_missing"), &LuaASTNode::is_missing);
	ClassDB::bind_method(D_METHOD("is_extra"), &LuaASTNode::is_extra);
	ClassDB::bind_method(D_METHOD("has_errors"), &LuaASTNode::has_errors);
	ClassDB::bind_method(D_METHOD("is_error"), &LuaASTNode::is_error);
	ClassDB::bind_method(D_METHOD("get_child", "index"), &LuaASTNode::get_child);
	ClassDB::bind_method(D_METHOD("get_child_count"), &LuaASTNode::get_child_count);
	ClassDB::bind_method(D_METHOD("get_named_child", "index"), &LuaASTNode::get_named_child);
	ClassDB::bind_method(D_METHOD("get_named_child_count"), &LuaASTNode::get_named_child_count);
	ClassDB::bind_method(D_METHOD("get_child_by_field_name", "field_name"), &LuaASTNode::get_child_by_field_name);
	ClassDB::bind_method(D_METHOD("get_parent"), &LuaASTNode::get_parent);
	ClassDB::bind_method(D_METHOD("get_next_sibling"), &LuaASTNode::get_next_sibling);
	ClassDB::bind_method(D_METHOD("get_previous_sibling"), &LuaASTNode::get_previous_sibling);
	ClassDB::bind_method(D_METHOD("get_next_named_sibling"), &LuaASTNode::get_next_named_sibling);
	ClassDB::bind_method(D_METHOD("get_previous_named_sibling"), &LuaASTNode::get_previous_named_sibling);
	ClassDB::bind_method(D_METHOD("get_source_code"), &LuaASTNode::get_source_code);
	ClassDB::bind_method(D_METHOD("get_tree"), &LuaASTNode::get_tree);
	ClassDB::bind_method(D_METHOD("query", "query"), &LuaASTNode::query);
}

String LuaASTNode::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class(), get_instance_id());
}

}
