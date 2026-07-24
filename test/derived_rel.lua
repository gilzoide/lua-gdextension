local DerivedRel = {
	class_name = "DerivedRel",
	extends = "base.lua",
}

function DerivedRel:shadow()
	return true
end

function DerivedRel:test()
	return self.base_property == 123 and self:base_func() == 456
end

return DerivedRel
