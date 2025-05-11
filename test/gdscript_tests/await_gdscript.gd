extends RefCounted


signal some_signal(value)


func test_await_gdscript_signal() -> bool:
	var lua = LuaState.new()
	lua.open_libraries()
	
	lua.globals.some_signal = some_signal
	var coroutine = lua.do_string("""
		return coroutine.create(function()
			await(some_signal)
		end)
	""")
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	coroutine.resume()
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)

	some_signal.emit()
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)

	return true


func test_await_gdscript_coroutine() -> bool:
	var lua = LuaState.new()
	lua.open_libraries()
	
	lua.globals._gdscript_coroutine = _gdscript_coroutine
	var coroutine = lua.do_string("""
		return coroutine.create(function()
			await(_gdscript_coroutine())
		end)
	""")
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	coroutine.resume()
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)

	some_signal.emit()
	assert(coroutine.status == LuaCoroutine.STATUS_DEAD)

	return true


func _gdscript_coroutine():
	await some_signal
