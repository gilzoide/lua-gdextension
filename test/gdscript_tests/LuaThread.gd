extends RefCounted


var lua_state: LuaState


func _init():
	lua_state = LuaState.new()


func test_main_thread() -> bool:
	var main_thread = lua_state.main_thread
	assert(main_thread is LuaThread)
	assert(main_thread.is_main_thread())
	return true
