assert(GlobalNodeScript, "Could not access GDScript from its class_name")
assert(Variant.is(GlobalNodeScript, Script), "Global class is not a Script")

--- This test usually works, but in the CI it fails because the first time Godot editor opens it crashes.
-- assert(LuaBouncingLogo, "Could not access LuaScript from its class_name")
-- assert(Variant.is(LuaBouncingLogo, Script), "Global class is not a Script")
