local TestClass = {}

TestClass.init_values = Array()
TestClass.some_signal = signal("arg1", "arg2")

function TestClass:_init(...)
	self.init_values = Array{ ... }
end

function TestClass:send_signal(arg1, arg2)
	self.some_signal:emit(arg1 or 1, arg2 or 2)
end

return TestClass
