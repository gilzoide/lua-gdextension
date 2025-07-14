local dict = Dictionary()
assert(not dict:is_typed())
assert(dict:get_typed_key_builtin() == TYPE_NIL)
assert(dict:get_typed_value_builtin() == TYPE_NIL)
assert(dict:get_typed_key_class_name() == "")
assert(dict:get_typed_value_class_name() == "")

-- Variant typed dictionary
local IntKeyedDict = Dictionary[int]
local int_keyed_dict = IntKeyedDict { "1", 2, false }
assert(int_keyed_dict:is_typed())
assert(int_keyed_dict:get_typed_key_builtin() == TYPE_INT)
assert(int_keyed_dict:get_typed_value_builtin() == TYPE_NIL)

-- Class typed dictionary
local NodeValuedDict = Dictionary[Variant][Node]
local node_valued_dict = NodeValuedDict()
assert(node_valued_dict:is_typed())
assert(node_valued_dict:get_typed_key_builtin() == TYPE_NIL)
assert(node_valued_dict:get_typed_value_builtin() == TYPE_OBJECT)
assert(node_valued_dict:get_typed_key_class_name() == "")
assert(node_valued_dict:get_typed_value_class_name() == "Node")

-- Script typed array
local BouncingLogo = ResourceLoader:load("res://bouncing_logo.lua")
local BouncingLogoValuedDict = Dictionary[Variant][BouncingLogo]
local bouncing_logo_valued_dict = BouncingLogoValuedDict()
assert(bouncing_logo_valued_dict:is_typed())
assert(bouncing_logo_valued_dict:get_typed_key_builtin() == TYPE_NIL)
assert(bouncing_logo_valued_dict:get_typed_key_class_name() == "")
assert(bouncing_logo_valued_dict:get_typed_value_builtin() == TYPE_OBJECT)
assert(bouncing_logo_valued_dict:get_typed_value_class_name() == BouncingLogo:get_instance_base_type())
assert(bouncing_logo_valued_dict:get_typed_value_script() == BouncingLogo)
