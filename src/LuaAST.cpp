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
#include "LuaAST.hpp"

#include "LuaASTNode.hpp"

#include <tree_sitter/api.h>

using namespace godot;

namespace luagdextension {

LuaAST::LuaAST() {
	ERR_FAIL_MSG("FIXME: LuaAST should never be instanced with default constructor");
}
LuaAST::LuaAST(TSTree *tree, const String& source_code)
	: tree(tree)
	, source_code(source_code)
{
}
LuaAST::~LuaAST() {
	ts_tree_delete(tree);
}

Ref<LuaASTNode> LuaAST::get_root() const {
	return memnew(LuaASTNode(const_cast<LuaAST*>(this), ts_tree_root_node(tree)));
}

String LuaAST::get_source_code() const {
	return source_code;
}

bool LuaAST::has_errors() const {
	return ts_node_has_error(ts_tree_root_node(tree));
}

void LuaAST::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_root"), &LuaAST::get_root);
	ClassDB::bind_method(D_METHOD("get_source_code"), &LuaAST::get_source_code);
	ClassDB::bind_method(D_METHOD("has_errors"), &LuaAST::has_errors);
	ADD_PROPERTY(PropertyInfo(Variant::Type::OBJECT, "root", godot::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY, LuaASTNode::get_class_static()), "", "get_root");
	ADD_PROPERTY(PropertyInfo(Variant::Type::STRING, "source_code", godot::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_source_code");
}

}
