local TestClass = {}

TestClass.some_signal = signal("arg1", "arg2")
TestClass.empty_array = property { type = Array }
TestClass.preinitialized_array = Array { 1, 2, 3 }

-- Getter methods
TestClass.getter_counter = property {
	get = function(self)
		self._count = (self._count or 0) + 1
		return self._count
	end,
}
TestClass.getter_name = property { get = "get_a" }

function TestClass:get_a()
	return self.a or "a"
end

function TestClass:_get(name)
	if name == "_get_some_property" then
		return "_get_some_property"
	end
end

-- Setter methods
TestClass.setter_increment = property {
	set = function(self, value)
		self:rawset("setter_increment", value + 1)
	end,
}
TestClass.setter_name = property { set = "set_a", }

function TestClass:set_a(value)
	self.a = value
end

function TestClass:_set(key, value)
	if key == "_set_some_property" then
		self:rawset(key, value)
		return true
	end
end

-- Initialization with new(...)
function TestClass:_init(...)
	self.init_values = Array { ... }
end

function TestClass:send_signal(arg1, arg2)
	self.some_signal:emit(arg1 or 1, arg2 or 2)
end

function TestClass:echo(value)
	return value
end

return TestClass
