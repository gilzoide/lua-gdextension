local DerivedAbs = {
	class_name = "DerivedAbs",
	extends = "res://base.lua",
}

function DerivedAbs:shadow()
	return true
end

function DerivedAbs:test()
	return self.base_property == 123 and self:base_func() == 456
end

return DerivedAbs
