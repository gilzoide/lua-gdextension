extends RefCounted

var lua_state
var initial_package_path
var initial_package_cpath


func _init():
	lua_state = LuaState.new()
	lua_state.open_libraries()
	initial_package_path = lua_state.package_path
	initial_package_cpath = lua_state.package_cpath


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


func test_package_path_default() -> bool:
	lua_state.package_path = ""
	assert(lua_state.package_path == "")
	lua_state.package_path = ";;"
	assert(lua_state.package_path == initial_package_path)
	lua_state.package_path = "prefix;;"
	assert(lua_state.package_path == "prefix;" + initial_package_path)
	lua_state.package_path = ";;suffix"
	assert(lua_state.package_path == initial_package_path + ";suffix")
	lua_state.package_path = "prefix;;suffix"
	assert(lua_state.package_path == "prefix;" + initial_package_path + ";suffix")
	return true


func test_package_cpath_default() -> bool:
	lua_state.package_cpath = ""
	assert(lua_state.package_cpath == "")
	lua_state.package_cpath = ";;"
	assert(lua_state.package_cpath == initial_package_cpath)
	lua_state.package_cpath = "prefix;;"
	assert(lua_state.package_cpath == "prefix;" + initial_package_cpath)
	lua_state.package_cpath = ";;suffix"
	assert(lua_state.package_cpath == initial_package_cpath + ";suffix")
	lua_state.package_cpath = "prefix;;suffix"
	assert(lua_state.package_cpath == "prefix;" + initial_package_cpath + ";suffix")
	return true
