local v = Vector2()
assert(v:length(), "Method call without arguments failed")
assert(v:angle_to(v) == 0, "Method call with valid arguments failed")

assert(not pcall(function() return v:angle_to() end), "Method call with invalid arguments succeeded")

-- Special case of "duplicate", which should invoke existing methods if possible
local node = Node:new()
local node_duplicate = node:duplicate()
node:call_deferred("queue_free")
assert(Variant.is(node_duplicate, Node))
assert(not is_same(node, node_duplicate))
node_duplicate:call_deferred("queue_free")
