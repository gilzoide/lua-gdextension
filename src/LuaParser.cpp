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
#include "LuaParser.hpp"

#include "LuaAST.hpp"
#include "utils/tree_sitter_lua.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace luagdextension {

// Input functions
static const char *read_func(void *payload, uint32_t byte_index, TSPoint position, uint32_t *bytes_read) {
	String *s = (String *) payload;
	if (byte_index == 0) {
		*bytes_read = s->length() * sizeof(char32_t);
		return (const char *) s->ptr();
	}
	else {
		*bytes_read = 0;
		return nullptr;
	}
}
static uint32_t decode_func(const uint8_t *string, uint32_t length, int32_t *code_point) {
	const char32_t *utf32_string = (const char32_t *) string;
	*code_point = *utf32_string;
	return sizeof(char32_t);
}

// Memory functions
static void *ts_alloc(size_t size) {
	return memalloc(size);
}
static void *ts_calloc(size_t count, size_t size) {
	size_t total = count * size;
	void *ptr = memalloc(total);
	memset(ptr, 0, total);
	return ptr;
}
static void *ts_realloc(void *ptr, size_t size) {
	return memrealloc(ptr, size);
}
static void ts_free(void *ptr) {
	memfree(ptr);
}

// Log function
void ts_log(void *payload, TSLogType log_type, const char *buffer) {
	switch (log_type) {
		case TSLogTypeParse:
			UtilityFunctions::print("[LuaParser][Parse] ", buffer);
			break;

		case TSLogTypeLex:
			UtilityFunctions::print("[LuaParser][Lex] ", buffer);
			break;
	}
}

// LuaParser
LuaParser::LuaParser()
	: parser(ts_parser_new())
{
	ts_parser_set_language(parser, tree_sitter_lua());
}
LuaParser::~LuaParser() {
	ts_parser_delete(parser);
}

Ref<LuaAST> LuaParser::parse_code(const String& code) const {
	TSInput input = {
		(void *) &code,
		read_func,
		TSInputEncodingCustom,
		decode_func,
	};
	
	if (TSTree *tree = ts_parser_parse(parser, nullptr, input)) {
		return memnew(LuaAST(tree, code));
	}
	else {
		return nullptr;
	}
}

bool LuaParser::get_debug_log() const {
	return ts_parser_logger(parser).log != nullptr;
}
void LuaParser::set_debug_log(bool enabled) {
	ts_parser_set_logger(parser, {
		nullptr,
		enabled ? ts_log : nullptr,
	});
}

void LuaParser::setup_tree_sitter_allocator() {
	ts_set_allocator(
		ts_alloc,
		ts_calloc,
		ts_realloc,
		ts_free
	);
}

void LuaParser::_bind_methods() {
	ClassDB::bind_method(D_METHOD("parse_code", "code"), &LuaParser::parse_code);
	ClassDB::bind_method(D_METHOD("set_debug_log", "enabled"), &LuaParser::set_debug_log);
	ClassDB::bind_method(D_METHOD("get_debug_log"), &LuaParser::get_debug_log);
	ADD_PROPERTY(PropertyInfo(Variant::Type::BOOL, "debug_log"), "set_debug_log", "get_debug_log");
}

String LuaParser::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

}
