local callable = Callable(OS, "get_name")
assert(callable() == OS:get_name())
