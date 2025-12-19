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
#include "convert_godot_std.hpp"

#include <godot_cpp/classes/object.hpp>

namespace luagdextension {

std::string to_std_string(const String& s) {
	if (s.is_empty()) {
		return "";
	}

	CharString chars = s.utf8();
	return std::string(chars.ptr(), chars.size());
}

std::string_view to_string_view(const PackedByteArray& bytes) {
	return std::string_view((const char *) bytes.ptr(), bytes.size());
}

String get_type_name(const Variant& variant) {
	Variant::Type type = variant.get_type();
	if (type == Variant::OBJECT) {
		Object *object = variant;
		return object->get_class();
	}
	else {
		return Variant::get_type_name(type);
	}
}

const char *get_operator_name(Variant::Operator op) {
	switch (op) {
		case Variant::OP_EQUAL:
			return "==";
		case Variant::OP_NOT_EQUAL:
			return "~=";
		case Variant::OP_LESS:
			return "<";
		case Variant::OP_LESS_EQUAL:
			return "<=";
		case Variant::OP_GREATER:
			return ">";
		case Variant::OP_GREATER_EQUAL:
			return ">=";
		case Variant::OP_ADD:
			return "+";
		case Variant::OP_SUBTRACT:
			return "-";
		case Variant::OP_MULTIPLY:
			return "*";
		case Variant::OP_DIVIDE:
			return "/";
		case Variant::OP_NEGATE:
			return "-";
		case Variant::OP_POSITIVE:
			return "+";
		case Variant::OP_MODULE:
			return "%";
		case Variant::OP_SHIFT_LEFT:
			return "<<";
		case Variant::OP_SHIFT_RIGHT:
			return ">>";
		case Variant::OP_BIT_AND:
			return "&";
		case Variant::OP_BIT_OR:
			return "|";
		case Variant::OP_BIT_XOR:
			return "^";
		case Variant::OP_BIT_NEGATE:
			return "~";
		case Variant::OP_AND:
			return "and";
		case Variant::OP_OR:
			return "or";
		case Variant::OP_XOR:
			return "~";
		case Variant::OP_NOT:
			return "not";
		case Variant::OP_IN:
			return "in";
		default:
			return "";
	}
}

String to_string(const GDExtensionCallError& call_error) {
	switch (call_error.error) {
        case GDEXTENSION_CALL_ERROR_INVALID_METHOD:
			return "Invalid method";
		case GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT:
			return "Invalid argument";
		case GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS:
			return "Too many arguments";
		case GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS:
			return "Too few arguments";
		case GDEXTENSION_CALL_ERROR_INSTANCE_IS_NULL:
			return "Instance is null";
		case GDEXTENSION_CALL_ERROR_METHOD_NOT_CONST:
			return "Method is not const";
		case GDEXTENSION_CALL_OK:
		default:
			return "";
	}
}

}

