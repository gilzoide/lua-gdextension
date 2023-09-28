extends RefCounted


func test() -> bool:
	var lua_state = LuaState.new()
	lua_state.open_libraries()
	var function = lua_state.do_string("""
		local DEFAULT_VALUE = 42
		return function(...)
			if select('#', ...) == 0 then
				return DEFAULT_VALUE
			else
				return ...
			end
		end
	""")
	assert(not function is LuaError, "Lua code failed: %s" % function)
	
	var result = function.invoke()
	assert(not result is LuaError, "Lua function call failed: %s" % result)
	assert(result == 42, "Default value '42' was not returned")

	assert(test_call(function, [5]) == 5, "Value passed '5' was not returned")
	assert(test_call(function, [1, 2]) == [1, 2], "Multiple returns did not return array")
	assert(test_call(function, [1, 2, {}]) == [1, 2, {}], "Multiple returns did not return array")
	return true


func test_call(f: LuaFunction, args: Array = []):
	var result = f.invokev(args)
	if result is LuaError:
		assert(false, "Lua function call failed: %s" % result)
	return result
