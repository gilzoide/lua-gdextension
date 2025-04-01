extends RefCounted


func test_libraries_opened() -> bool:
	var lua_state = LuaState.new()
	assert(not lua_state.are_libraries_opened(LuaState.LUA_BASE))
	lua_state.open_libraries(LuaState.LUA_BASE | LuaState.LUA_PACKAGE)
	assert(lua_state.are_libraries_opened(LuaState.LUA_BASE))
	assert(lua_state.are_libraries_opened(LuaState.LUA_PACKAGE))
	assert(lua_state.are_libraries_opened(LuaState.LUA_BASE | LuaState.LUA_PACKAGE))
	assert(not lua_state.are_libraries_opened(LuaState.LUA_ALL_LIBS))
	return true
