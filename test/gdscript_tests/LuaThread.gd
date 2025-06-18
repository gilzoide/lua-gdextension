extends RefCounted


var lua_state: LuaState
var _hook_call_count := 0


func _setup():
	lua_state = LuaState.new()
	lua_state.open_libraries()
	_hook_call_count = 0


func test_main_thread() -> bool:
	var main_thread = lua_state.main_thread
	assert(main_thread is LuaThread)
	assert(main_thread.is_main_thread())
	return true


func test_set_hook() -> bool:
	var main_thread = lua_state.main_thread
	main_thread.set_hook(_func_call_hook, LuaThread.HOOK_MASK_CALL)
	assert(_hook_call_count == 0)
	lua_state.do_string("")
	assert(_hook_call_count == 1)
	return true


func test_get_hook() -> bool:
	var main_thread = lua_state.main_thread
	assert(main_thread.get_hook() == null)
	assert(main_thread.get_hook_mask() == 0)
	assert(main_thread.get_hook_count() == 0)
	
	# Callable hook
	main_thread.set_hook(_func_call_hook, LuaThread.HOOK_MASK_CALL, 3)
	assert(main_thread.get_hook() == _func_call_hook)
	assert(main_thread.get_hook_mask() == LuaThread.HOOK_MASK_CALL)
	assert(main_thread.get_hook_count() == 3)

	# LuaFunction hook
	var hook_lua_func = lua_state.create_function(_func_call_hook)
	main_thread.set_hook(hook_lua_func, LuaThread.HOOK_MASK_CALL, 3)
	assert(main_thread.get_hook() == hook_lua_func)
	assert(main_thread.get_hook_mask() == LuaThread.HOOK_MASK_CALL)
	assert(main_thread.get_hook_count() == 3)
	
	main_thread.set_hook(null, 0)
	assert(main_thread.get_hook() == null)
	assert(main_thread.get_hook_mask() == 0)
	assert(main_thread.get_hook_count() == 0)
	return true


func _func_call_hook(debug):
	_hook_call_count += 1
