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
#include "LuaCoroutine.hpp"
#include "LuaError.hpp"
#include "LuaFunction.hpp"
#include "LuaLightUserdata.hpp"
#include "LuaObject.hpp"
#include "LuaState.hpp"
#include "LuaTable.hpp"
#include "LuaUserdata.hpp"
#include "script-language/LuaScriptExtension.hpp"
#include "script-language/LuaScriptLanguageExtension.hpp"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;
using namespace luagdextension;

static void initialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Lua object wrappers
	ClassDB::register_abstract_class<LuaObject>();

	ClassDB::register_abstract_class<LuaCoroutine>();
	ClassDB::register_abstract_class<LuaFunction>();
	ClassDB::register_abstract_class<LuaLightUserdata>();
	ClassDB::register_abstract_class<LuaTable>();
	ClassDB::register_abstract_class<LuaUserdata>();

	// Godot classes for interacting with Lua States
	ClassDB::register_class<LuaError>();
	ClassDB::register_class<LuaState>();

	// Lua Script Language
	ClassDB::register_abstract_class<LuaScriptExtension>();
	ClassDB::register_abstract_class<LuaScriptLanguageExtension>();
	LuaScriptLanguageExtension::get_or_create_singleton();
}

static void deinitialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Lua Script Language
	LuaScriptLanguageExtension::delete_singleton();
}

extern "C" GDExtensionBool luagdextension_entrypoint(
	const GDExtensionInterfaceGetProcAddress p_getprocaccess,
	GDExtensionClassLibraryPtr p_library,
	GDExtensionInitialization *r_initialization
) {
	GDExtensionBinding::InitObject init_obj(p_getprocaccess, p_library, r_initialization);

	init_obj.register_initializer(&initialize);
	init_obj.register_terminator(&deinitialize);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
