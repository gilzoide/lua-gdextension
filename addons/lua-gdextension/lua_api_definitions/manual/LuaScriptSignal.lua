--- @meta
--- @diagnostic disable: undefined-doc-name, duplicate-doc-field, duplicate-doc-alias

--- @class LuaScriptSignal
-- Signal definition for Lua scripts.
LuaScriptSignal = {}

--- @return bool
function LuaScriptSignal:is_null() end

--- @return Object
function LuaScriptSignal:get_object() end

--- @return number
function LuaScriptSignal:get_object_id() end

--- @return StringName
function LuaScriptSignal:get_name() end

--- @return number
--- @param callable Callable
--- @param flags number? @default `0`
function LuaScriptSignal:connect(callable, flags) end

--- @param callable Callable
function LuaScriptSignal:disconnect(callable) end

--- @return bool
--- @param callable Callable
function LuaScriptSignal:is_connected(callable) end

--- @return Array
function LuaScriptSignal:get_connections() end

--- @return bool
function LuaScriptSignal:has_connections() end

function LuaScriptSignal:emit(...) end

--- @return LuaScriptSignal
--- @param ... string
-- Used to define custom signals in Lua scripts.
--
-- For now there is no way to pass type information for arguments, only their names.
-- ```lua
-- -- Lua
-- local MyClass = {}
-- MyClass.some_signal = signal("argument1", "argument2")
-- return MyClass
-- -- End Lua
-- ```
function signal(...) end
