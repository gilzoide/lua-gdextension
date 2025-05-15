assert(is_same(1, 1))
assert(not is_same(Array(), Array()))
assert(lerp(1, 2, 0.5) == 1.5)

-- variadic functions
assert(min(1, 3, 4, 7, 20, 55) == 1)
assert(str(1, 2, 3, "hello!") == "123hello!")
