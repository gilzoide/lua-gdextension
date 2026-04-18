local Derived = {
	class_name = "Derived",
	extends = "Base",
}

function Derived:test()
	return self.base_property == 123 and self:base_func() == 456
end

function Derived:_ready()
	print(self.base_property)
	print(self:base_func())
	print(Derived:test())
end

return Derived
