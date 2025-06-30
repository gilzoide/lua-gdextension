extends RefCounted


func _some_callable():
	pass


func test_clear_G() -> bool:
	var lua = LuaState.new()
	lua.globals.clear()
	lua.globals.test = _some_callable
	return true
