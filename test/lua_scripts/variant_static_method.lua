local b = assert(Basis:from_euler(Vector3()), "Static method call failed")
local size = assert(String:humanize_size(42), "Static method call failed")

assert(not pcall(function() Basis:from_euler("invalid argument") end), "Static method call with invalid argument succeeded")
