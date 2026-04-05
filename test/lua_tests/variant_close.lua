-- Avoid running test if to-be-closed variables is not supported
if not coroutine.close then
	return
end

-- Now for the actual test...

local variant = RefCounted:new()
assert(Variant.is(variant, RefCounted))
do
	-- Use load to avoid parser errors in LuaJIT
	load("local v<close> = ...")(variant)
end
assert(Variant.is(variant, nil))
