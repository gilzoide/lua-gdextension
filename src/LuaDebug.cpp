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
#include "LuaDebug.hpp"

namespace luagdextension {

LuaDebug::LuaDebug() : debug() {}
LuaDebug::LuaDebug(const lua_Debug& debug) : debug(debug) {}
LuaDebug::LuaDebug(lua_Debug&& debug) : debug(std::move(debug)) {}

LuaThread::HookEvent LuaDebug::get_event() const {
	return (LuaThread::HookEvent) debug.event;
}

String LuaDebug::get_name() const {
	return debug.name;
}

String LuaDebug::get_name_what() const {
	return debug.namewhat;
}

String LuaDebug::get_what() const {
	return debug.what;
}

String LuaDebug::get_source() const {
	return debug.source;
}

String LuaDebug::get_short_src() const {
	return debug.short_src;
}

int LuaDebug::get_line_defined() const {
	return debug.linedefined;
}

int LuaDebug::get_last_line_defined() const {
	return debug.lastlinedefined;
}

int LuaDebug::get_current_line() const {
	return debug.currentline;
}

#if LUA_VERSION_NUM >= 502
int LuaDebug::get_nparams() const {
	return debug.nparams;
}

bool LuaDebug::is_tail_call() const {
	return debug.istailcall;
}

bool LuaDebug::is_vararg() const {
	return debug.isvararg;
}
#endif

void LuaDebug::fill_info(lua_State *L, lua_Debug *ar) {
#if LUA_VERSION_NUM >= 502
	lua_getinfo(L, "nSltu", ar);
#else
	lua_getinfo(L, "nSlu", ar);
#endif
}

void LuaDebug::fill_info(const sol::protected_function& f, lua_Debug *ar) {
	f.push();
	lua_getinfo(f.lua_state(), ">nSu", ar);
}

void LuaDebug::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_event"), &LuaDebug::get_event);
	ClassDB::bind_method(D_METHOD("get_name"), &LuaDebug::get_name);
	ClassDB::bind_method(D_METHOD("get_name_what"), &LuaDebug::get_name_what);
	ClassDB::bind_method(D_METHOD("get_what"), &LuaDebug::get_what);
	ClassDB::bind_method(D_METHOD("get_source"), &LuaDebug::get_source);
	ClassDB::bind_method(D_METHOD("get_short_src"), &LuaDebug::get_short_src);
	ClassDB::bind_method(D_METHOD("get_line_defined"), &LuaDebug::get_line_defined);
	ClassDB::bind_method(D_METHOD("get_last_line_defined"), &LuaDebug::get_last_line_defined);
	ClassDB::bind_method(D_METHOD("get_current_line"), &LuaDebug::get_current_line);
#if LUA_VERSION_NUM >= 502
	ClassDB::bind_method(D_METHOD("get_nparams"), &LuaDebug::get_nparams);
	ClassDB::bind_method(D_METHOD("is_tail_call"), &LuaDebug::is_tail_call);
	ClassDB::bind_method(D_METHOD("is_vararg"), &LuaDebug::is_vararg);
#endif

	ADD_PROPERTY(PropertyInfo(Variant::INT, "event", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CLASS_IS_ENUM, "LuaThread::HookEvent"), "", "get_event");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name_what", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_name_what");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "what", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_what");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "source", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_source");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "short_src", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_short_src");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "line_defined", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_line_defined");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "last_line_defined", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_last_line_defined");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_line", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_current_line");
#if LUA_VERSION_NUM >= 502
	ADD_PROPERTY(PropertyInfo(Variant::INT, "nparams", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_nparams");
#endif
}

String LuaDebug::_to_string() const {
	return String("[%s:%d]") % Array::make(get_class_static(), get_instance_id());
}

}
