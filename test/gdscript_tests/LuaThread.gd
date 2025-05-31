extends RefCounted


var lua_state: LuaState
var hook_call_count := 0


func _setup():
	lua_state = LuaState.new()
	hook_call_count = 0


func test_main_thread() -> bool:
	var main_thread = lua_state.main_thread
	assert(main_thread is LuaThread)
	assert(main_thread.is_main_thread())
	return true


func test_set_hook() -> bool:
	var main_thread = lua_state.main_thread
	main_thread.set_hook(_func_call_hook, LuaThread.HOOK_MASK_CALL)
	lua_state.do_string("")
	assert(hook_call_count == 1)
	return true


func test_get_hook() -> bool:
	var main_thread = lua_state.main_thread
	assert(not main_thread.get_hook().is_valid())
	assert(main_thread.get_hook_mask() == 0)
	assert(main_thread.get_hook_count() == 0)
	
	main_thread.set_hook(_func_call_hook, LuaThread.HOOK_MASK_CALL, 3)
	assert(not main_thread.get_hook() == _func_call_hook)
	assert(main_thread.get_hook_mask() == LuaThread.HOOK_MASK_CALL)
	assert(main_thread.get_hook_count() == 3)
	
	main_thread.set_hook(Callable(), 0)
	assert(not main_thread.get_hook().is_valid())
	assert(main_thread.get_hook_mask() == 0)
	assert(main_thread.get_hook_count() == 0)
	return true


func _func_call_hook():
	hook_call_count += 1
