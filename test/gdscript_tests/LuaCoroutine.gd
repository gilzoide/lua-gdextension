extends RefCounted


var lua_state: LuaState
var yielding_function: LuaFunction


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


func test_create():
	var coroutine = LuaCoroutine.create(yielding_function)
	assert(coroutine is LuaCoroutine)
	return true


func test_status():
	var coroutine = LuaCoroutine.create(yielding_function)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	coroutine.resume()
	assert(coroutine.status == LuaCoroutine.STATUS_OK)
	return true


func test_send_and_receive_arguments():
	var coroutine = LuaCoroutine.create(yielding_function)
	for i in range(0, 10):
		var result = coroutine.resume(i)
		assert(result == i)
	assert(coroutine.status == LuaCoroutine.STATUS_YIELD)
	assert(coroutine.resume() == null)
	assert(coroutine.status == LuaCoroutine.STATUS_OK)
	return true
