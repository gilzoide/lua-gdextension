extends Node

var test_class_node = load("res://gdscript_tests/lua_files/test_class_node.lua")


func test_rpc_call() -> bool:
	var obj = test_class_node.new()
	add_child(obj)
	obj.rpc("rpc_method")
	assert(obj.rpc_called)
	return true
