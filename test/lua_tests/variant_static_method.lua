local b = assert(Basis:from_euler(Vector3()), "Static method call failed")
local size = assert(String:humanize_size(42), "Static method call failed")

assert(not pcall(function() Basis:from_euler("invalid argument") end), "Static method call with invalid argument succeeded")

assert(Variant.get_type(1) == int)
assert(Variant.get_type(1.1) == float)
assert(Variant.get_type(Vector2()) == Vector2)

assert(Variant.is("some string", String))

assert(Variant.booleanize(1))
assert(not Variant.booleanize(0))
