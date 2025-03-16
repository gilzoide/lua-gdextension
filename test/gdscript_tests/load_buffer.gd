extends RefCounted


var lua_state
var function


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()


func test_load_fail() -> bool:
	var result = lua_state.load_buffer("this is an invalid Lua script".to_ascii_buffer())
	assert(result is LuaError, "load_buffer with invalid Lua script should return LuaError")
	return true


func test_load_function() -> bool:
	var result = lua_state.load_buffer("return ...".to_ascii_buffer())
	assert(result is LuaFunction, "load_buffer with valid Lua script should return LuaFunction")
	assert(result.invoke(1) == 1)
	assert(result.invoke(2) == 2)
	assert(result.invoke(1, 2, 3) == [1, 2, 3])
	return true


func test_load_mode() -> bool:
	var result = lua_state.load_buffer("return ...".to_ascii_buffer(), "", LuaState.LOAD_MODE_TEXT)
	assert(result is LuaFunction, "load_buffer with valid Lua script should return LuaFunction")
	result = lua_state.load_buffer("return ...".to_ascii_buffer(), "", LuaState.LOAD_MODE_BINARY)
	assert(result is LuaError, "load_buffer loading a text chunk in binary mode should return LuaError")
	return true


func test_load_function_env() -> bool:
	var env = lua_state.create_table()
	var result = lua_state.load_buffer("value = 42".to_ascii_buffer(), "", LuaState.LOAD_MODE_TEXT, env)
	assert(result is LuaFunction, "load_buffer with valid Lua script should return LuaFunction")
	assert(result.invoke() == null)
	assert(env.value == 42)
	return true
