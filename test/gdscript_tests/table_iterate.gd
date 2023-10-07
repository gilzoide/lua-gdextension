extends RefCounted

var lua_state = LuaState.new()

func test_empty() -> bool:
	return _test_n(0)


func test_1() -> bool:
	return _test_n(1)


func test_10() -> bool:
	return _test_n(10)


func _test_n(n: int) -> bool:
	var table = lua_state.create_table()
	
	for i in n:
		table.set(str("key", i), str("Hello ", i))
	
	var iteration_count = 0
	for k in table:
		iteration_count += 1
	assert(iteration_count == n, "Iterated %d times over LuaTable with %d items" % [iteration_count, n])
	return true
