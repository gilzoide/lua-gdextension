--- @meta

--- Yields the current coroutine until the passed signal is emitted.
--- If an Object is passed, awaits for its 'completed' signal.
--- This function should only be called inside a coroutine.
--- 
--- Note: only available if `GODOT_UTILITY_FUNCTIONS` library is open in the LuaState.
--- @param awaitable Object | Signal
--- @return any
function await(awaitable) end


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
