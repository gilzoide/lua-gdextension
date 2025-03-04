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
	for i in range(1, table.length() + 1):
		assert(table.get(i) == i)
	assert(table.get("hello") == "world")
	assert(table.get("invalid") == null)
	assert(table.get("invalid_with_defaul", "default") == "default")
	assert(table.get("some_vector2") == Vector2())
	assert(table.get(table) == "self")
	return true


func test_set_value() -> bool:
	var table = lua_state.create_table()
	table.set(1, 1)
	table.set(2, 2)
	table.set(3, 3)
	table.set(4, 4)
	assert(table.length() == 4)
	return true


func test_clear() -> bool:
	var table = lua_state.create_table()
	table.set(1, 1)
	table.set(2, 2)
	table.set(3, 3)
	table.set(4, 4)
	table.clear()
	assert(table.length() == 0)
	for k in table:
		assert(false, "Cleared table should have no key/value pair")
	return true
