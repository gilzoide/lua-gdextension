local callable = Callable(OS, "get_name")
assert(callable() == OS:get_name())
local a = 1
local custom_callable = Callable(function(val)
	a = a + val
	print(a == 6 and "Passed" or "Failed")
end)
custom_callable:call_deferred(5)
