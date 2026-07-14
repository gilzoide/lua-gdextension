--- @meta
--- @diagnostic disable: undefined-doc-name, duplicate-doc-field, duplicate-doc-alias

--- @class LuaScriptProperty
-- Property definition for Lua scripts.
LuaScriptProperty = {}

--- @return LuaScriptProperty
--- @param t table | any
-- Used to define custom properties in Lua scripts.
-- 
-- If you pass a table, the following keys are used (all are optional):
-- 
-- + `1`: if it's a Variant type or Class (like `Dictionary` or `Node2D`) it represents the property type.
-- 
--   Otherwise, it represents the property's default value.
-- 
-- + `type`: should be a Variant type or a Class, such as `Vector2` or `RefCounted`.
-- 
-- + `hint`: property hint (check out the `PropertyHint` enum for available values)
-- 
-- + `hint_string`: property hint string, depends on the value of `hint`
-- 
-- + `usage`: property usage flags (check out the `PropertyUsage` enum for available values)
-- 
-- + `class_name`: the name of the Class, filled automatically from `type` if it's a Class type
-- 
-- + `default`: the default value of the property
-- 
-- + `get`: getter function, should be either a Lua function or a string containing the getter method name
-- 
-- + `set`: setter function, should be either a Lua function or a string containing the setter method name
-- 
-- In case `t` is not a table, the table `{t}` will be used instead.
function property(t) end

--- @return LuaScriptProperty
-- Same as `property`, but always adds `PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE` to the property's usage flags.
-- 
-- Similar to GDScript's `@export` annotation. Has subtables for property hints and usage.
-- 
--- @see property
function export() end

--- @return LuaScriptProperty
-- Creates a `PROPERTY_USAGE_CATEGORY` property.
-- 
-- Note that the category name will be the key used in your class for this property.
function export_category() end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_COLOR_NO_ALPHA` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_color_no_alpha` annotation.
-- 
--- @see export
function export_color_no_alpha(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_DIR` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_dir` annotation.
-- 
--- @see export
function export_dir(t) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_ENUM` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_enum` annotation.
-- 
--- @see export
function export_enum(...) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_EXP_EASING` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_exp_easing` annotation.
-- 
--- @see export
function export_exp_easing(...) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_FILE` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_file` annotation.
-- 
--- @see export
function export_file(...) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_FLAGS` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_flags` annotation.
-- 
--- @see export
function export_flags(...) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_NAVIGATION` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_2d_navigation` annotation.
-- 
--- @see export
function export_flags_2d_navigation(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_PHYSICS` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_2d_physics` annotation.
-- 
--- @see export
function export_flags_2d_physics(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_2D_RENDER` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_2d_render` annotation.
-- 
--- @see export
function export_flags_2d_render(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_NAVIGATION` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_3d_navigation` annotation.
-- 
--- @see export
function export_flags_3d_navigation(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_PHYSICS` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_3d_physics` annotation.
-- 
--- @see export
function export_flags_3d_physics(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_3D_RENDER` to the property's usage flags.
-- 
-- Similar to GDScript's `@export_flags_3d_render` annotation.
-- 
--- @see export
function export_flags_3d_render(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_LAYERS_AVOIDANCE` to the property's usage flags.
--
-- Similar to GDScript's `@export_flags_avoidance` annotation.
--
--- @see export
function export_flags_avoidance(t) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_GLOBAL_DIR` to the property's usage flags.
--
-- Similar to GDScript's `@export_global_dir` annotation.
--
--- @see export
function export_global_dir(t) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_GLOBAL_FILE` to the property's hint flags.
--
-- String arguments will be used as the property's `hint_string`.
--
-- The first argument that is not a string is forwarded to `export`
--
-- Similar to GDScript's `export_global_file` annotation.
--
--- @see export
function export_global_file(...) end

--- @return LuaScriptProperty
--- @param prefix string?
-- Creates a `PROPERTY_USAGE_GROUP` property.
-- 
-- Note that the group name will be the key used in your class for this property.
function export_group(prefix) end

--- @return LuaScriptProperty
--- @param t table | any
-- Same as `export`, but always adds `PROPERTY_HINT_MULTILINE_TEXT` to the property's usage flags.
--
-- Similar to GDScript's `@export_multiline` annotation.
--
--- @see export
function export_multiline(t) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_NODE_PATH_VALID_TYPES` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_node_path` annotation.
-- 
--- @see export
function export_node_path(...) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_PLACEHOLDER_TEXT` to the property's hint flags.
-- 
-- String arguments will be used as the property's `hint_string`.
-- 
-- The first argument that is not a string is forwarded to `export`
-- 
-- Similar to GDScript's `export_placeholder` annotation.
--
--- @see export
function export_placeholder(...) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_RANGE` to the property's hint flags.
-- 
-- The first argument that is not a number or string is forwarded to `export`.
-- 
-- Similar to GDScript's `export_node_path` annotation.
-- 
--- @see export
function export_range(...) end

--- @return LuaScriptProperty
--- @param t table | any
-- Used to define exported properties in Lua scripts.
-- 
-- This is the same as `property`, but always adds `PROPERTY_USAGE_STORAGE` to the property's usage flags.
-- 
--- @see property
function export_storage(t) end

--- @return LuaScriptProperty
--- @param prefix string?
-- Creates a `PROPERTY_USAGE_SUBGROUP` property.
-- 
-- Note that the subgroup name will be the key used in your class for this property.
function export_subgroup(prefix) end

--- @return LuaScriptProperty
--- @param ... string | any
-- Same as `export`, but always adds `PROPERTY_HINT_TOOL_BUTTON` to the property's hint flags.
-- 
-- The first argument that is not a string is forwarded to `export`.
-- 
-- Similar to GDScript's `export_node_path` annotation.
-- 
--- @see export
function export_tool_button(...) end