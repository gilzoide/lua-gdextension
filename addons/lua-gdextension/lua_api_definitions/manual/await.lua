--- @meta
--- @diagnostic disable: undefined-doc-name, duplicate-doc-field, duplicate-doc-alias

--- Yields the current coroutine until the passed signal is emitted.
--- If an Object is passed, awaits for its 'completed' signal.
--- This function should only be called inside a coroutine.
--- 
--- Note: only available if `GODOT_UTILITY_FUNCTIONS` library is open in the LuaState.
--- @param awaitable Object | Signal
--- @return any
function await(awaitable) end
