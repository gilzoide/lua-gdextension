/**
 * Copyright (C) 2023 Gil Barbosa Reis.
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
#ifndef __LUA_TABLE_HPP__
#define __LUA_TABLE_HPP__

#include "godot_utils.hpp"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <sol/sol.hpp>

using namespace godot;

namespace luagdextension {

class LuaTable : public RefCounted {
	GDCLASS(LuaTable, RefCounted);

public:
	LuaTable();
	LuaTable(sol::table&& table);
	LuaTable(const sol::table& table);

	Variant geti(int64_t index) const;
	void seti(int64_t index, const Variant& value);
	size_t size() const;

	Dictionary to_dictionary() const;
	Array to_array() const;

protected:
	static void _bind_methods();
	
	bool _get(const StringName& property_name, Variant& r_value) const;
	bool _set(const StringName& property_name, const Variant& value);
	String _to_string() const;

	sol::table table;

private:
	template<typename TKey>
	void _settable(const TKey& key, const Variant& value) {
		switch (value.get_type()) {
			case Variant::BOOL:
				table[key] = (bool) value;
				break;

			case Variant::INT:
				table[key] = (int64_t) value;
				break;

			case Variant::FLOAT:
				table[key] = (double) value;
				break;

			case Variant::STRING:
			case Variant::STRING_NAME:
			case Variant::NODE_PATH:
				table[key] = to_std_string((String) value);
				break;

			case Variant::VECTOR2:
			case Variant::VECTOR2I:
			case Variant::RECT2:
			case Variant::RECT2I:
			case Variant::VECTOR3:
			case Variant::VECTOR3I:
			case Variant::TRANSFORM2D:
			case Variant::VECTOR4:
			case Variant::VECTOR4I:
			case Variant::PLANE:
			case Variant::QUATERNION:
			case Variant::AABB:
			case Variant::BASIS:
			case Variant::TRANSFORM3D:
			case Variant::PROJECTION:
			case Variant::COLOR:
			case Variant::RID:
			case Variant::OBJECT:
			case Variant::CALLABLE:
			case Variant::SIGNAL:
			case Variant::DICTIONARY:
			case Variant::ARRAY:
			case Variant::PACKED_BYTE_ARRAY:
			case Variant::PACKED_INT32_ARRAY:
			case Variant::PACKED_INT64_ARRAY:
			case Variant::PACKED_FLOAT32_ARRAY:
			case Variant::PACKED_FLOAT64_ARRAY:
			case Variant::PACKED_STRING_ARRAY:
			case Variant::PACKED_VECTOR2_ARRAY:
			case Variant::PACKED_VECTOR3_ARRAY:
			case Variant::PACKED_COLOR_ARRAY:
				WARN_PRINT_ONCE_ED("Lua type not yet supported");

			case Variant::NIL:
			default:
				table[key] = sol::nil;
				break;
		}
	}
};

}

#endif
