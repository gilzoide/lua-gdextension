---@type Node
local derived = Derived:new()
local derived_abs = DerivedAbs:new()
local derived_rel = DerivedRel:new()

local function test(derived)
	assert(derived:test())
	assert(derived:shadow())
	assert(derived:get_script():get_base_script():get_global_name() == "Base")
	do
		local check = false
		local methods = derived:get_method_list()
		for i = 0, #methods - 1 do
			local v = methods[i]
			if v.name == "shadow" then
				check = true
				break
			end
		end
		assert(check)
	end
	do
		local check = false
		local signals = derived:get_signal_list()
		for i = 0, #signals - 1 do
			local v = signals[i]
			if v.name == "signal_test" then
				check = true
				break
			end
		end
		assert(check)
	end
	do
		local check = false
		local properties = derived:get_script():get_script_property_list()
		for i = 0, #properties - 1 do
			local v = properties[i]
			if v.name == "base_property" then
				check = true
				break
			end
		end
		assert(check)
	end
end

test(derived)
test(derived_abs)
test(derived_rel)
