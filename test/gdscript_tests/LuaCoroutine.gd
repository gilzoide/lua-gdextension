extends RefCounted


var lua_state: LuaState
var yielding_function: LuaFunction
var _signal_handled = false


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
