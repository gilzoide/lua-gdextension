local obj = assert(RefCounted:new(), "Constructing class failed")

assert(not pcall(function() return LuaObject:new() end), "Instantiating abstract class should raise an error")
