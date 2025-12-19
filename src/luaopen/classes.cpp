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

#include "../utils/Class.hpp"
#include "../utils/module_names.hpp"
#include "../utils/convert_godot_std.hpp"

#include <godot_cpp/classes/project_settings.hpp>
#include <sol/sol.hpp>

using namespace luagdextension;

extern "C" int luaopen_godot_classes(lua_State *L) {
	sol::state_view state = L;

	state.registry()[module_names::classes] = true;
	sol::table global_class_paths = state.registry().create_named("_GDEXTENSION_GLOBAL_CLASS_PATHS");
	Class::register_usertype(state);

	TypedArray<Dictionary> global_class_list = ProjectSettings::get_singleton()->get_global_class_list();
	for (int64_t i = 0; i < global_class_list.size(); ++i) {
		Dictionary type_info = global_class_list[i];
		global_class_paths[to_std_string(type_info["class"])] = to_std_string(type_info["path"]);
	}

	return 0;
}
