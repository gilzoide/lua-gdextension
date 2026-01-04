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
#include "Class.hpp"

#include "convert_godot_lua.hpp"
#include "method_bind_impl.hpp"
#include "string_names.hpp"

#include <godot_cpp/classes/class_db_singleton.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/godot.hpp>

namespace luagdextension {

Class::Class(const StringName& class_name)
	: class_name(class_name)
{
}

const StringName& Class::get_name() const {
	return class_name;
}

sol::optional<int64_t> Class::get_constant(const StringName& name) const {
	ClassDBSingleton *class_db = ClassDBSingleton::get_singleton();
	if (class_db->class_has_integer_constant(class_name, name)) {
		return class_db->class_get_integer_constant(class_name, name);
	}
	else {
		return sol::nullopt;
	}
}

Variant Class::construct(sol::this_state state, const sol::variadic_args& args) const {
	auto class_db = ClassDBSingleton::get_singleton();
	if (!class_db->can_instantiate(class_name)) {
		luaL_error(
			state,
			"Class '%s' or its base class cannot be instantiated",
			String(class_name).ascii().get_data()
		);
	}
	
	Variant new_obj = class_db->instantiate(class_name);
	if (new_obj.has_method(string_names->_init)) {
		variant_call_string_name(state, new_obj, string_names->_init, args);
	}
	return new_obj;
}

bool Class::operator==(const Class& other) const {
	return class_name == other.class_name;
}

static sol::object __index(sol::this_state state, const Class& cls, sol::stack_object key) {
	if (key.get_type() == sol::type::string) {
		StringName name = key.as<StringName>();
		if (auto constant = cls.get_constant(name)) {
			return sol::make_object(state, *constant);
		}
		else if (ClassDBSingleton::get_singleton()->class_has_method(cls.get_name(), name)) {
			return sol::make_object(state, ClassMethodBind(cls, name));
		}
	}
	return sol::nil;
}
void Class::register_usertype(sol::state_view& state) {
	state.new_usertype<Class>(
		"Class",
		"new", &Class::construct,
		sol::meta_function::index, &__index,
		sol::meta_function::to_string, &Class::get_name
	);
	ClassMethodBind::register_usertype(state);
}

}
