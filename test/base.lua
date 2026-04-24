---@class Base : Node
local Base = {
	class_name = "Base",
	extends = Node,
	abstract = true,
	base_property = 123,
	signal_test = signal()
}

function Base:shadow()
	return false
end

function Base:base_func()
	return 456
end

return Base
