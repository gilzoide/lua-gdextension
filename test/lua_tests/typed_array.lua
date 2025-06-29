local array = Array()
assert(not array:is_typed())
assert(array:get_typed_builtin() == TYPE_NIL)
assert(array:get_typed_class_name() == "")

-- Variant typed array
local IntArray = Array[int]
local int_array = IntArray { 1, 2, 3 }
assert(int_array:is_typed())
assert(int_array:get_typed_builtin() == TYPE_INT)

-- Class typed array
local NodeArray = Array[Node]
local node_array = NodeArray()
assert(node_array:is_typed())
assert(node_array:get_typed_builtin() == TYPE_OBJECT)
assert(node_array:get_typed_class_name() == "Node")

-- Script typed array
local BouncingLogo = ResourceLoader:load("res://bouncing_logo.lua")
local BouncingLogoArray = Array[BouncingLogo]
local bouncing_logo_array = BouncingLogoArray()
assert(bouncing_logo_array:is_typed())
assert(bouncing_logo_array:get_typed_builtin() == TYPE_OBJECT)
assert(bouncing_logo_array:get_typed_class_name() == BouncingLogo:get_instance_base_type())
assert(bouncing_logo_array:get_typed_script() == BouncingLogo)
