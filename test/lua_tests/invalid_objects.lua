assert(instance_from_id(0) == nil)

assert(not is_instance_valid(nil))
local node = Node:new()
assert(is_instance_valid(node))
node:free()
assert(not is_instance_valid(node))

