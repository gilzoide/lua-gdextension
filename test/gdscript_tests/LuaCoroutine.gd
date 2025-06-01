extends RefCounted


var lua_state: LuaState
var yielding_function: LuaFunction
var _signal_handled := false
var _hook_call_count := 0


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()
	yielding_function = lua_state.do_string("""
		return function(value)
			while value do
				value = coroutine.yield(value)
			end
		end
	""")


func _setup():
	_signal_handled = false
	_hook_call_count = 0


func _create_coroutine_lua() -> LuaCoroutine:
	return lua_state.do_string("""
		return coroutine.create(function(value)
			while value do
				value = coroutine.yield(value)
			end
		end)
	""")


func _create_failing_coroutine_lua() -> LuaCoroutine:
	return lua_state.do_string("""
		return coroutine.create(function(value)
			error("failed!")
		end)
	""")


func _handle_signal(ret):
	_signal_handled = true


func test_create() -> bool:
	var coroutine = LuaCoroutine.create(yielding_function)
	assert(coroutine is LuaCoroutine)
	return true


func test_create_lua() -> bool:
	var coroutine = _create_coroutine_lua()
	assert(coroutine is LuaCoroutine)
	return true


func test_status() -> bool:
	var coroutine = LuaCoroutine.create(yielding_function)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	coroutine.resume()
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_status_lua() -> bool:
	var coroutine = _create_coroutine_lua()
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	coroutine.resume()
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_send_and_receive_arguments() -> bool:
	var coroutine = LuaCoroutine.create(yielding_function)
	for i in range(0, 10):
		var result = coroutine.resume(i)
		assert(result == i)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	assert(coroutine.resume() == null)
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_send_and_receive_arguments_lua() -> bool:
	var coroutine = _create_coroutine_lua()
	for i in range(0, 10):
		var result = coroutine.resume(i)
		assert(result == i)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	assert(coroutine.resume() == null)
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_send_and_receive_array_arguments() -> bool:
	var coroutine = LuaCoroutine.create(yielding_function)
	for i in range(0, 10):
		var result = coroutine.resumev([i])
		assert(result == i)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	assert(coroutine.resumev([]) == null)
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_send_and_receive_array_arguments_lua() -> bool:
	var coroutine = _create_coroutine_lua()
	for i in range(0, 10):
		var result = coroutine.resumev([i])
		assert(result == i)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	assert(coroutine.resumev([]) == null)
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)
	return true


func test_completed_signal() -> bool:
	var coroutine = _create_coroutine_lua()
	coroutine.completed.connect(_handle_signal)
	assert(not _signal_handled)
	coroutine.resume()
	assert(_signal_handled)
	return true


func test_failed_signal() -> bool:
	var coroutine = _create_failing_coroutine_lua()
	coroutine.failed.connect(_handle_signal)
	assert(not _signal_handled)
	coroutine.resume()
	assert(_signal_handled)
	return true


func test_set_hook() -> bool:
	var coroutine = _create_coroutine_lua()
	coroutine.set_hook(_func_call_hook, LuaThread.HOOK_MASK_CALL)
	assert(_hook_call_count == 0)
	coroutine.resume()
	assert(_hook_call_count == 1)
	return true


func test_hook_yield() -> bool:
	var coroutine = LuaCoroutine.create(lua_state.load_string("""
		global = 1
		global = 2
		global = 3
	"""))
	coroutine.set_hook(_func_line_hook, LuaThread.HOOK_MASK_LINE)
	coroutine.resume()
	# yielded at first line, before running "global = 1"
	assert(lua_state.globals.global == null)
	coroutine.resume()
	# yielded at second line, before running "global = 2"
	assert(lua_state.globals.global == 1)
	coroutine.resume()
	# yielded at third line, before running "global = 3"
	assert(lua_state.globals.global == 2)
	return true


func _func_call_hook(debug):
	_hook_call_count += 1


func _func_line_hook(debug):
	return LuaThread.HOOK_YIELD
