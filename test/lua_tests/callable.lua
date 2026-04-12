local callable = Callable(OS, "get_name")
assert(callable() == OS:get_name())
local a = 1
local custom_callable = Callable(function(val)
	a = a + val
	return a
end)
local result = custom_callable(5)
assert(a == 6 and result == 6)
custom_callable:bind(1)
custom_callable()
assert(a == 7)
