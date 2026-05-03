---@type Base
local base = Base:new()
---@type Base
local derived = Derived:new()
---@type Base
local derived_abs = DerivedAbs:new()
---@type Base
local derived_rel = DerivedRel:new()

local function derived_test(derived)
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

derived_test(derived)
derived_test(derived_abs)
derived_test(derived_rel)

local function base_test(base)
	assert(not base.test)
	assert(not base:shadow())
	assert(not base:get_script():get_base_script())
	local check = false
	local properties = base:get_script():get_script_property_list()
	for i = 0, #properties - 1 do
		local v = properties[i]
		if v.name == "base_property" then
			check = true
			break
		end
	end
	for i = 0, #properties - 1 do
		local v = properties[i]
		if v.name == "derived_only" then
			check = false
			break
		end
	end
	assert(check)
end

base_test(base)
