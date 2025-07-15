-- Typed arrays
-- Element type: int
local int_array = Array[int]()
-- Element type: Node
local node_array = Array[Node]()

-- Construct from table
local array = Array{ "value 0", "value 1" }
-- Godot Arrays are indexed from 0, instead of 1
print(array[0])  -- "value 0"
print(array[1])  -- "value 1"
print(array[2])  -- nil

-- Length
print(array:size())  -- 2
print(#array)        -- 2

-- Runtime Variant type checking (also works for other variant types)
print(Variant.is(array, Array))    -- true
print(Variant.is(array, "Array"))  -- true
-- Method syntax makes this a bit shorter
print(array:is(Array))       -- true
print(array:is(Dictionary))  -- false
print(array:is(RefCounted))  -- false
