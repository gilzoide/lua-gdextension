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

--- Used to define exported properties in Lua scripts.
--- This is the same as `property`, but always adds `PROPERTY_USAGE_EDITOR` to the property's usage flags.
--- 
--- @see property
--- @param t table | any
--- @return LuaScriptProperty
function export(t) end


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

--- Similar to GDScript's `@rpc` annotation, should be used to initialize the special `rpc_config`.
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
