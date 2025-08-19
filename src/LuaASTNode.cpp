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
#include "LuaASTNode.hpp"

#include <tree_sitter/api.h>

using namespace godot;

namespace luagdextension {

LuaASTNode::LuaASTNode() {
	ERR_FAIL_MSG("FIXME: LuaASTNode should never be instanced with default constructor");
}
LuaASTNode::LuaASTNode(TSNode node)
	: node(node)
{
}

bool LuaASTNode::has_errors() const {
	return ts_node_has_error(node);
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

void LuaASTNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("has_errors"), &LuaASTNode::has_errors);
	ClassDB::bind_method(D_METHOD("dump"), &LuaASTNode::dump);
}

String LuaASTNode::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

}
