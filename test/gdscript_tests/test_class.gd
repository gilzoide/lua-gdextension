extends RefCounted

var test_class = load("res://gdscript_tests/lua_files/test_class.lua")
var _signal_handled = false


func _handle_signal(arg1, arg2):
	_signal_handled = true


func test_init() -> bool:
	var obj = test_class.new(1, 2, 3)
	assert(obj is RefCounted, "Object is not RefCounted")
	assert(obj.get_script() is LuaScript, "Object should have LuaScript attached")
	assert(obj.init_values == [1, 2, 3])
	return true


func test_signal() -> bool:
	var obj = test_class.new()
	assert(obj.some_signal is Signal, "Object should have signal defined in script")
	obj.some_signal.connect(_handle_signal)
	obj.send_signal()
	assert(_signal_handled, "Signal was not handled")
	return true;


func test_array_properties() -> bool:
	var obj = test_class.new()
	assert(obj.empty_array == [])
	assert(!is_same(obj.empty_array, test_class.get_property_default_value("empty_array")), "Properties should be duplicated on instantiation")
	assert(obj.preinitialized_array == [1, 2, 3])
	assert(!is_same(obj.preinitialized_array, test_class.get_property_default_value("preinitialized_array")), "Properties should be duplicated on instantiation")
	return true
