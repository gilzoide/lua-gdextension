extends RefCounted

var test_class = load("res://gdscript_tests/lua_files/test_class.lua")
var _signal_handled = false


func _handle_signal(arg1, arg2):
	_signal_handled = true


func test_init() -> bool:
	var obj = test_class.new(1, 2, 3)
	assert(obj is RefCounted, "Object is not RefCounted")
	assert(obj.get_script() is LuaScript, "Object should have LuaScript attached")
	assert(obj.init_values != null)
	return true


func test_signal() -> bool:
	var obj = test_class.new()
	assert(obj.some_signal is Signal, "Object should have signal defined in script")
	obj.some_signal.connect(_handle_signal)
	obj.send_signal()
	assert(_signal_handled, "Signal was not handled")
	return true;


func test_property_duplicate() -> bool:
	var obj = test_class.new()
	assert(!is_same(obj.init_values, obj.get_script().get_property_default_value("init_values")), "Properties should be duplicated on instantiation")
	return true
