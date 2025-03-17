extends RefCounted

var lua_state


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()


func test_package_path() -> bool:
	var package_path = "res://?.lua"
	lua_state.package_path = package_path
	assert(lua_state.package_path == package_path)
	assert(lua_state.globals.package.path == package_path)
	assert(lua_state.globals.package.cpath != package_path)
	return true


func test_package_cpath() -> bool:
	var package_cpath = "!/?.so"
	lua_state.package_cpath = package_cpath
	assert(lua_state.package_cpath == package_cpath)
	assert(lua_state.globals.package.path != package_cpath)
	assert(lua_state.globals.package.cpath == package_cpath)
	return true
