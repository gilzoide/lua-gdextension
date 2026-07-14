--- @meta
--- @diagnostic disable: undefined-doc-name, duplicate-doc-field, duplicate-doc-alias

--- Creates a table suitable for defining Godot Classes in Lua scripts.
--- The only thing special about it is that `pairs` iterates over its keys in order of insertion,
--- so that its properties and methods are shown in order of definition in the Godot Editor.
--- @return table
function GDCLASS() end