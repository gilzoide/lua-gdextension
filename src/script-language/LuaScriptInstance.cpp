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
#include <godot_cpp/classes/script.hpp>

#include "LuaScriptInstance.hpp"

#include "LuaScriptLanguage.hpp"
#include "godot_cpp/core/memory.hpp"

namespace luagdextension {

LuaScriptInstance::LuaScriptInstance(Object *owner, const LuaScript *script) : owner(owner), script(script) {}

GDExtensionScriptInstanceSet set_func;
GDExtensionScriptInstanceGet get_func;
GDExtensionScriptInstanceGetPropertyList get_property_list_func;
GDExtensionScriptInstanceFreePropertyList free_property_list_func;

GDExtensionScriptInstancePropertyCanRevert property_can_revert_func;
GDExtensionScriptInstancePropertyGetRevert property_get_revert_func;

void *get_owner_func(LuaScriptInstance *instance) {
	return instance->owner;
}

GDExtensionScriptInstanceGetPropertyState get_property_state_func;

GDExtensionScriptInstanceGetMethodList get_method_list_func;
GDExtensionScriptInstanceFreeMethodList free_method_list_func;
GDExtensionScriptInstanceGetPropertyType get_property_type_func;

GDExtensionScriptInstanceHasMethod has_method_func;

GDExtensionScriptInstanceCall call_func;
GDExtensionScriptInstanceNotification notification_func;

GDExtensionScriptInstanceToString to_string_func;

GDExtensionScriptInstanceRefCountIncremented refcount_incremented_func;
GDExtensionScriptInstanceRefCountDecremented refcount_decremented_func;

void *get_script_func(LuaScriptInstance *instance) {
	return (void *) instance->script;
}

GDExtensionScriptInstanceIsPlaceholder is_placeholder_func;

GDExtensionScriptInstanceSet set_fallback_func;
GDExtensionScriptInstanceGet get_fallback_func;

void *get_language_func(LuaScriptInstance *instance) {
	return LuaScriptLanguage::get_singleton();
}

void free_func(LuaScriptInstance *instance) {
	memfree(instance);
}

GDExtensionScriptInstanceInfo script_instance_info = {
	(GDExtensionScriptInstanceSet) set_func,
	(GDExtensionScriptInstanceGet) get_func,
	(GDExtensionScriptInstanceGetPropertyList) get_property_list_func,
	(GDExtensionScriptInstanceFreePropertyList) free_property_list_func,
	(GDExtensionScriptInstancePropertyCanRevert) property_can_revert_func,
	(GDExtensionScriptInstancePropertyGetRevert) property_get_revert_func,
	(GDExtensionScriptInstanceGetOwner) get_owner_func,
	(GDExtensionScriptInstanceGetPropertyState) get_property_state_func,
	(GDExtensionScriptInstanceGetMethodList) get_method_list_func,
	(GDExtensionScriptInstanceFreeMethodList) free_method_list_func,
	(GDExtensionScriptInstanceGetPropertyType) get_property_type_func,
	(GDExtensionScriptInstanceHasMethod) has_method_func,
	(GDExtensionScriptInstanceCall) call_func,
	(GDExtensionScriptInstanceNotification) notification_func,
	(GDExtensionScriptInstanceToString) to_string_func,
	(GDExtensionScriptInstanceRefCountIncremented) refcount_incremented_func,
	(GDExtensionScriptInstanceRefCountDecremented) refcount_decremented_func,
	(GDExtensionScriptInstanceGetScript) get_script_func,
	(GDExtensionScriptInstanceIsPlaceholder) is_placeholder_func,
	(GDExtensionScriptInstanceSet) set_fallback_func,
	(GDExtensionScriptInstanceGet) get_fallback_func,
	(GDExtensionScriptInstanceGetLanguage) get_language_func,
	(GDExtensionScriptInstanceFree) free_func,
};
GDExtensionScriptInstanceInfo *LuaScriptInstance::get_script_instance_info() {
	return &script_instance_info;
}

}
