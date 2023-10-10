extends RefCounted

var table


func _init():
	var lua_state = LuaState.new()
	lua_state.open_libraries()
	table = lua_state.create_table()


func test_table() -> bool:
	assert(table.size() == 0)
	return true
