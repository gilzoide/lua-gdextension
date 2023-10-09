extends RefCounted

var lua_state


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()


func test_metatable_tostring() -> bool:
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


func test_get_value() -> bool:
	var table = lua_state.do_string("""
		local t = { 1, 2, 3, 4,
			hello = "world",
			some_vector2 = Vector2(),

		}
		t[t] = "self"
		return t
	""")
	assert(table is LuaTable)
	for i in range(1, table.size() + 1):
		assert(table.get_value(i) == i)
	assert(table.get_value("hello") == "world")
	assert(table.get_value("invalid") == null)
	assert(table.get_value("invalid_with_defaul", "default") == "default")
	assert(table.get_value("some_vector2") == Vector2())
	assert(table.get_value(table) == "self")
	return true


func test_set_value() -> bool:
	var table = lua_state.create_table()
	table.set_value(1, 1)
	table.set_value(2, 2)
	table.set_value(3, 3)
	table.set_value(4, 4)
	assert(table.size() == 4)
	return true
