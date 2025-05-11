extends RefCounted


var lua_state: LuaState


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()


func test_is_same_table() -> bool:
	lua_state.do_string("""
		t = {}
	""")
	var t = lua_state.globals["t"]
	assert(lua_state.globals["t"] == t)
	assert(is_same(lua_state.globals["t"], t))
	return true


func test_is_same_coroutine() -> bool:
	lua_state.do_string("""
		c = coroutine.create(function() end)
	""")
	var c = lua_state.globals["c"]
	assert(lua_state.globals["c"] == c)
	assert(is_same(lua_state.globals["c"], c))
	return true


func test_is_same_function() -> bool:
	lua_state.do_string("""
		f = function() end
	""")
	var f = lua_state.globals["f"]
	assert(lua_state.globals["f"] == f)
	assert(is_same(lua_state.globals["f"], f))
	return true
