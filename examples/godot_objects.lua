-- Instantiate Godot objects
local v = Vector3(1, 2, 3)
print(v.x)  -- 1

-- Note: use ":" instead of "." to call methods in Lua
print(v:length())  -- 3.74165749549866

local n = Node:new()
print(n:is_inside_tree())  -- false
n:queue_free()
