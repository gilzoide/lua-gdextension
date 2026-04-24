---@type Node
local derived = Derived:new()
assert(derived:test())
assert(derived:shadow())
assert(derived:get_script():get_base_script():get_global_name() == "Base")
print(derived:get_method_list())
print(derived:get_signal_list())
print(derived:get_property_list())
