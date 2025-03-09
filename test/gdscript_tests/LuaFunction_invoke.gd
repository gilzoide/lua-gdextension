extends RefCounted


var lua_state
var function


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()
	function = lua_state.do_string("""
		local DEFAULT_VALUE = 42
		return function(...)
			if select('#', ...) == 0 then
				return DEFAULT_VALUE
			else
				return ...
			end
		end
	""")


func test_function_created() -> bool:
	assert(not function is LuaError, "Lua code failed: %s" % function)
	return true


func test_invoke_no_args() -> bool:
	var result = function.invoke()
	assert(not result is LuaError, "Lua function call failed: %s" % result)
	assert(result == 42, "Default value '42' was not returned")
	return true


func test_invoke_args() -> bool:
	assert(_test_call(function, [5]) == 5, "Value passed '5' was not returned")
	assert(_test_call(function, [1, 2]) == [1, 2], "Multiple returns did not return array")
	assert(_test_call(function, [1, 2, {}]) == [1, 2, {}], "Multiple returns did not return array")
	return true


func test_callable() -> bool:
	var callable = function.to_callable()
	assert(callable.call(5) == 5, "Value passed '5' was not returned")
	assert(callable.call(1, 2) == [1, 2], "Multiple returns did not return array")
	assert(callable.call(1, 2, []) == [1, 2, []], "Multiple returns did not return array")
	return true


func _test_call(f: LuaFunction, args: Array = []):
	var result = f.invokev(args)
	if result is LuaError:
		assert(false, "Lua function call failed: %s" % result)
	return result
