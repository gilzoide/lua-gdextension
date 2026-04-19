local Derived = {
	class_name = "Derived",
	extends = "Base",
}

function Derived:test()
	return self.base_property == 123 and self:base_func() == 456
end

return Derived
