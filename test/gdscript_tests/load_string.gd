extends RefCounted


var lua_state
var function


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()


func test_load_fail() -> bool:
	var result = lua_state.load_string("this is an invalid Lua script")
	assert(result is LuaError, "load_string with invalid Lua script should return LuaError")
	return true


func test_load_function() -> bool:
	var result = lua_state.load_string("return ...")
	assert(result is LuaFunction, "load_string with valid Lua script should return LuaFunction")
	assert(result.invoke(1) == 1)
	assert(result.invoke(2) == 2)
	assert(result.invoke(1, 2, 3) == [1, 2, 3])
	return true
