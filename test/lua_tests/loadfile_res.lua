local f = assert(loadfile("res://lua_tests/return_args.lua"))
assert(type(f) == "function")
assert(f(1) == 1)

assert(not loadfile("res://non-existent-file"))
