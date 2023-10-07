extends RefCounted


func test_metatable_tostring() -> bool:
	var lua_state = LuaState.new()
	lua_state.open_libraries()
	var table = lua_state.do_string("""
		function __tostring(self)
			return "hello " .. (self.name or "world")
		end
		return setmetatable({}, { __tostring = __tostring })
	""")
	assert(not table is LuaError, "Lua code failed: %s" % table)
	assert(str(table) == "hello world", "__tostring was not called")
	table.set("name", "gilzoide")
	assert(str(table) == "hello gilzoide", "__tostring was not called")
	return true

