local v = assert(Vector2())
v = assert(Vector2(1, 2))
v = assert(Vector2(v))

assert(not pcall(Vector2, "string"), "Invalid Variant constructor was successful")
assert(not pcall(Vector2, 1), "Invalid Variant constructor was successful")
