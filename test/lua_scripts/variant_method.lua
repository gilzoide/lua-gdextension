local v = Vector2()
assert(v:length(), "Method call without arguments failed")
assert(v:angle_to(v) == 0, "Method call with valid arguments failed")

assert(not pcall(function() return v:angle_to() end), "Method call with invalid arguments succeeded")
