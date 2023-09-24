local b = assert(Basis:from_euler(Vector3()), "Static method call failed")
local size = assert(String:humanize_size(42), "Static method call failed")

assert(not pcall(Basis.from_euler, Basis, "invalid argument"), "Static method call with invalid argument succeeded")
