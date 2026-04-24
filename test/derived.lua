local Derived = {
	class_name = "Derived",
	extends = "Base",
	derived_only = true
}

function Derived:shadow()
	return true
end

function Derived:test()
	return self.base_property == 123 and self:base_func() == 456
end

return Derived
