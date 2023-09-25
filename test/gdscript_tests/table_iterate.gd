extends RefCounted

var lua_state = LuaState.new()

func test():
	for i in range(0, 5):
		test_n(i)

func test_n(n: int):
	var table = lua_state.create_table()
	
	for i in n:
		table.set(str("key", i), str("Hello ", i))
	
	var iteration_count = 0
	for k in table:
		iteration_count += 1
	assert(iteration_count == n, "Iterated %d times over LuaTable with %d items" % [iteration_count, n])
