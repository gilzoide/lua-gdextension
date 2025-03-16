dofile("res://lua_tests/return_args.lua")

assert(not pcall(function() dofile("res://non-existent-file") end))
