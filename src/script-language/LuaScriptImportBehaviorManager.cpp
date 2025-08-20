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
#include "LuaScriptImportBehaviorManager.hpp"

#include "../utils/project_settings.hpp"

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_uid.hpp>

namespace luagdextension {

LuaScriptImportBehaviorManager::LuaScriptImportBehaviorManager()
	: map(ProjectSettings::get_singleton()->get_setting_with_override(LUA_SCRIPT_IMPORT_MAP_SETTING))
{
	prune_non_existent_uids();
}

void LuaScriptImportBehaviorManager::set_script_import_behavior(const String& script_path, int behavior) {
	int64_t uid = ResourceLoader::get_singleton()->get_resource_uid(script_path);
	if (uid >= 0) {
		String uid_str = ResourceUID::get_singleton()->id_to_text(uid);
		if (map.get(uid_str, nullptr) != Variant(behavior)) {
			if (behavior == 0) {
				map.erase(uid_str);
			}
			else {
				map[uid_str] = behavior;
			}
			save_map();
		}
	}
}

int LuaScriptImportBehaviorManager::get_script_import_behavior(const String& script_path) const {
	int64_t uid = ResourceLoader::get_singleton()->get_resource_uid(script_path);
	if (uid >= 0) {
		String uid_str = ResourceUID::get_singleton()->id_to_text(uid);
		return map.get(uid_str, 0);
	}
	else {
		return 0;
	}
}

void LuaScriptImportBehaviorManager::prune_non_existent_uids() {
	Array keys = map.keys();
	bool should_save = false;
	for (int64_t i = 0; i < keys.size(); i++) {
		String uid_str = keys[i];
		int64_t uid = ResourceUID::get_singleton()->text_to_id(uid_str);
		if (!ResourceUID::get_singleton()->has_id(uid)) {
			map.erase(uid_str);
			should_save = true;
		}
	}
	if (should_save) {
		save_map();
	}
}

LuaScriptImportBehaviorManager *LuaScriptImportBehaviorManager::get_singleton() {
	return instance;
}

LuaScriptImportBehaviorManager *LuaScriptImportBehaviorManager::get_or_create_singleton() {
	if (instance == nullptr) {
		instance = memnew(LuaScriptImportBehaviorManager);
	}
	return instance;
}

void LuaScriptImportBehaviorManager::delete_singleton() {
	memdelete(instance);
}

void LuaScriptImportBehaviorManager::save_map() {
	ProjectSettings::get_singleton()->set_setting(LUA_SCRIPT_IMPORT_MAP_SETTING_EDITOR, map);
	ProjectSettings::get_singleton()->save();
}

LuaScriptImportBehaviorManager *LuaScriptImportBehaviorManager::instance;

}
