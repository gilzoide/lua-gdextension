extends RefCounted


func test_sandboxed_callable() -> bool:
	var sum = func(arg1, arg2):
		return arg1 + arg2
	
	var lua_state = LuaState.new()
	lua_state.globals["sum"] = sum
	var result = lua_state.do_string("return sum(1, 2)")
	assert(result == 3)
	return true


func test_variant_callable() -> bool:
	var sum = func(arg1, arg2):
		return arg1 + arg2
	
	var lua_state = LuaState.new()
	lua_state.open_libraries()
	lua_state.globals["sum"] = sum
	var result = lua_state.do_string("return sum(1, 2)")
	assert(result == 3)
	return true
	