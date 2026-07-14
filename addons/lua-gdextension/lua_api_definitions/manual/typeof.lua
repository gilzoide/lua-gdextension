--- @meta
--- @diagnostic disable: undefined-doc-name, duplicate-doc-field, duplicate-doc-alias

--- Returns the Variant type of the passed value.
--- Contrary to GDScript's `typeof`, in Lua this does not return the enum like `TYPE_BOOL` or `TYPE_DICTIONARY`, but rather the actual class type like `bool` or `Dictionary`.
--- ```
--- if typeof(some_value) == Dictionary then
---   -- ...
--- end
--- ```
--- Note: only available if `GODOT_VARIANT` library is open in the LuaState.
--- @param value any
--- @return userdata?
function typeof(value) end