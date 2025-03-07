function test_iteration(n)
	local dict = Dictionary()
	for i = 1, n do
		dict[i] = "Hello " .. i
	end

	local iteration_count = 0
	for i, v in pairs(dict) do
		iteration_count = iteration_count + 1
	end
	assert(iteration_count == dict:size())
end

for i = 0, 5 do
	test_iteration(i)
end
