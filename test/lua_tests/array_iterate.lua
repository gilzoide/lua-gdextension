function test_iteration(n)
	local arr = Array()
	for i = 1, n do
		arr:append(i)
	end

	local iteration_count = 0
	for i, v in pairs(arr) do
		iteration_count = iteration_count + 1
	end
	assert(iteration_count == arr:size(), "Array pairs did not iterate over all " .. arr:size() .. " items")
end

for i = 0, 5 do
	test_iteration(i)
end
