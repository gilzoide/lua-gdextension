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
#ifndef __LUA_GDEXTENSION_STRING_NAMES_HPP__
#define __LUA_GDEXTENSION_STRING_NAMES_HPP__

#include <godot_cpp/variant/string_name.hpp>

using namespace godot;

namespace luagdextension {

struct string_names {
	// object creation / initialization
	StringName _init = "_init";
	StringName _new = "new";
	// script instance methods
	StringName _get = "_get";
	StringName _set = "_set";
	StringName _notification = "_notification";
	StringName _property_can_revert = "_property_can_revert";
	StringName _property_get_revert = "_property_get_revert";
	StringName _to_string = "_to_string";
	StringName _validate_property = "_validate_property";
	// CodeEdit
	StringName delimiter_comments = "delimiter_comments";
	StringName delimiter_strings = "delimiter_strings";
	// LuaScriptMetada
	StringName rawget = "rawget";
	StringName rawset = "rawset";
	// LuaCoroutine / await
	StringName completed = "completed";
	StringName failed = "failed";
	// LuaFunction
	StringName invoke = "invoke";
	// Variant.__length
	StringName size = "size";
	// MethodBindByName
	StringName class_call_static = "class_call_static";
};

extern string_names *string_names;

}

#endif  // __LUA_GDEXTENSION_STRING_NAMES_HPP__
