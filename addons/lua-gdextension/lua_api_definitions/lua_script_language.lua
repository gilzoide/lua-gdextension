--- @meta

-----------------------------------------------------------
-- Properties
-----------------------------------------------------------

--- @class LuaScriptProperty
--- Property definition for Lua scripts.
LuaScriptProperty = {}

--- Used to define custom properties in Lua scripts.
--- If you pass a table, the following keys are used (all are optional):
--- + `1`: if it's a Variant type or Class (like `Dictionary` or `Node2D`) it represents the property type.
---   Otherwise, it represents the property's default value.
--- + `type`: should be a Variant type or a Class, such as `Vector2` or `RefCounted`.
--- + `hint`: property hint (check out the `PropertyHint` enum for available values)
--- + `hint_string`: property hint string, depends on the value of `hint`
--- + `usage`: property usage flags (check out the `PropertyUsage` enum for available values)
--- + `class_name`: the name of the Class, filled automatically from `type` if it's a Class type
--- + `default`: the default value of the property
--- + `get`: getter function, should be either a Lua function or a string containing the getter method name
--- + `set`: setter function, should be either a Lua function or a string containing the setter method name
--- 
--- In case `t` is not a table, the table `{t}` will be used instead.
--- @param t table | any
--- @return LuaScriptProperty
function property(t) end

--- Same as `property`, but always adds `PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE` to the property's usage flags.
--- Similar to GDScript's `@export` annotation.
--- @see property
--- @param t table | any
--- @return LuaScriptProperty
function export(t) end

--- Creates a `PROPERTY_USAGE_CATEGORY` property.
--- Note that the category name will be the key used in your class for this property.
--- @return LuaScriptProperty
function export_category() end

--- Same as `export`, but always adds `PROPERTY_HINT_COLOR_NO_ALPHA` to the property's usage flags.
--- Similar to GDScript's `@export_color_no_alpha` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_color_no_alpha(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_DIR` to the property's usage flags.
--- Similar to GDScript's `@export_dir` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_dir(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_ENUM` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_enum` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_enum(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_EXP_EASING` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_exp_easing` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_exp_easing(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_FILE` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_file` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_file(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_FLAGS` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_flags` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_flags(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_NAVIGATION` to the property's usage flags.
--- Similar to GDScript's `@export_flags_2d_navigation` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_2d_navigation(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_PHYSICS` to the property's usage flags.
--- Similar to GDScript's `@export_flags_2d_physics` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_2d_physics(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_RENDER` to the property's usage flags.
--- Similar to GDScript's `@export_flags_2d_render` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_2d_render(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_NAVIGATION` to the property's usage flags.
--- Similar to GDScript's `@export_flags_3d_navigation` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_3d_navigation(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_PHYSICS` to the property's usage flags.
--- Similar to GDScript's `@export_flags_3d_physics` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_3d_physics(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_RENDER` to the property's usage flags.
--- Similar to GDScript's `@export_flags_3d_render` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_3d_render(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_AVOIDANCE` to the property's usage flags.
--- Similar to GDScript's `@export_flags_avoidance` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_flags_avoidance(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_GLOBAL_DIR` to the property's usage flags.
--- Similar to GDScript's `@export_global_dir` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_global_dir(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_GLOBAL_FILE` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_global_file` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_global_file(...) end

--- Creates a `PROPERTY_USAGE_GROUP` property.
--- Note that the group name will be the key used in your class for this property.
--- @param prefix string?
--- @return LuaScriptProperty
function export_group(prefix) end

--- Same as `export`, but always adds `PROPERTY_HINT_MULTILINE_TEXT` to the property's usage flags.
--- Similar to GDScript's `@export_multiline` annotation.
--- @see export
--- @param t table | any
--- @return LuaScriptProperty
function export_multiline(t) end

--- Same as `export`, but always adds `PROPERTY_HINT_NODE_PATH_VALID_TYPES` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_node_path` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_node_path(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_PLACEHOLDER_TEXT` to the property's hint flags.
--- String arguments will be used as the property's `hint_string`.
--- The first argument that is not a string is forwarded to `export`
--- Similar to GDScript's `export_placeholder` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_placeholder(...) end

--- Same as `export`, but always adds `PROPERTY_HINT_RANGE` to the property's hint flags.
--- The first argument that is not a number or string is forwarded to `export`.
--- Similar to GDScript's `export_node_path` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_range(...) end

--- Used to define exported properties in Lua scripts.
--- This is the same as `property`, but always adds `PROPERTY_USAGE_STORAGE` to the property's usage flags.
--- @see property
--- @param t table | any
--- @return LuaScriptProperty
function export_storage(t) end

--- Creates a `PROPERTY_USAGE_SUBGROUP` property.
--- Note that the subgroup name will be the key used in your class for this property.
--- @param prefix string?
--- @return LuaScriptProperty
function export_subgroup(prefix) end

--- Same as `export`, but always adds `PROPERTY_HINT_TOOL_BUTTON` to the property's hint flags.
--- The first argument that is not a string is forwarded to `export`.
--- Similar to GDScript's `export_node_path` annotation.
--- @see export
--- @param ... string | any
--- @return LuaScriptProperty
function export_tool_button(...) end


-----------------------------------------------------------
-- Signals
-----------------------------------------------------------

--- @class LuaScriptSignal
--- Signal definition for Lua scripts.
LuaScriptSignal = {}

--- Used to define custom signals in Lua scripts.
--- For now there is no way to pass type information for arguments, only their names.
--- ```
--- local MyClass = {}
--- MyClass.some_signal = signal("argument1", "argument2")
--- return MyClass
--- ```
--- @param ... string
--- @return LuaScriptSignal
function signal(...) end


-----------------------------------------------------------
-- RPC configuration
-----------------------------------------------------------

--- Similar to GDScript's `@rpc` annotation, should be used to initialize the special `rpc_config` table.
--- Example:
--- ```
--- local MyClass = {}
--- 
--- function MyClass:some_method() end
--- function MyClass:some_other_method() end
--- 
--- MyClass.rpc_config = {
---   "some_method" = rpc("any_peer", "call_local", "reliable", 0),
---   "some_other_method" = rpc("reliable", 1),
--- }
--- 
--- return MyClass
--- ```
--- See [@rpc](https://docs.godotengine.org/en/stable/classes/class_@gdscript.html#class-gdscript-annotation-rpc) for more information.
--- 
--- @param mode "any_peer" | "authority" | nil
--- @param sync "call_remote" | "call_local" | nil
--- @param transfer_mode "unreliable" | "unreliable_ordered" | "reliable" | nil
--- @param transfer_channel integer?
function rpc(mode, sync, transfer_mode, transfer_channel) end


-----------------------------------------------------------
-- Misc
-----------------------------------------------------------

--- Creates a table suitable for defining Godot Classes in Lua scripts.
--- The only thing special about it is that `pairs` iterates over its keys in order of insertion,
--- so that its properties and methods are shown in order of definition in the Godot Editor.
--- @return table
function GDCLASS() end
