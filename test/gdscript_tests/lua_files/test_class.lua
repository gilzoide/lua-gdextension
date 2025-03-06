local TestClass = {}

TestClass.some_signal = signal("arg1", "arg2")
TestClass.empty_array = property { type = Array }
TestClass.preinitialized_array = Array { 1, 2, 3 }

function TestClass:_init(...)
	self.init_values = Array{ ... }
end

function TestClass:send_signal(arg1, arg2)
	self.some_signal:emit(arg1 or 1, arg2 or 2)
end

return TestClass
