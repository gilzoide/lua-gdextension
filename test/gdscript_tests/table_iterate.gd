extends RefCounted

var lua_state = LuaState.new()

func test() -> bool:
	var all_success = true
	for i in range(0, 5):
		if not test_n(i):
			all_success = false
	return all_success

func test_n(n: int) -> bool:
	var table = lua_state.create_table()
	
	for i in n:
		table.set(str("key", i), str("Hello ", i))
	
	var iteration_count = 0
	for k in table:
		iteration_count += 1
	assert(iteration_count == n, "Iterated %d times over LuaTable with %d items" % [iteration_count, n])
	return true
