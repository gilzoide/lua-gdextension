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
	assert(not _signal_handled)
	obj.send_signal()
	assert(_signal_handled, "Signal was not handled")
	return true;


func test_array_properties() -> bool:
	var obj = test_class.new()
	var empty_array = obj.empty_array
	assert(obj.empty_array == [])
	assert(is_same(empty_array, obj.empty_array))
	assert(!is_same(obj.empty_array, test_class.get_property_default_value("empty_array")), "Properties should be duplicated on instantiation")
	assert(obj.preinitialized_array == [1, 2, 3])
	assert(!is_same(obj.preinitialized_array, test_class.get_property_default_value("preinitialized_array")), "Properties should be duplicated on instantiation")
	return true


func test_non_existent_property() -> bool:
	var obj = test_class.new()
	assert("some crazy non-existent property name" not in obj)
	return true;


func test_property_getter_function() -> bool:
	var obj = test_class.new()
	assert(obj.getter_counter == 1)
	assert(obj.getter_counter == 2)
	assert(obj.getter_counter == 3)
	return true;


func test_property_getter_name() -> bool:
	var obj = test_class.new()
	assert(obj.getter_name == "a")
	obj.a = "value"
	assert(obj.getter_name == "value")
	return true;


func test_property__get() -> bool:
	var obj = test_class.new()
	assert(obj.rawget("_get_some_property") == null)
	assert(obj._get_some_property == "_get_some_property")
	return true


func test_property_setter_function() -> bool:
	var obj = test_class.new()
	obj.setter_increment = 5
	assert(obj.setter_increment == 6)
	return true


func test_property_setter_name() -> bool:
	var obj = test_class.new()
	assert("a" not in obj)
	obj.setter_name = "value"
	assert(obj.setter_name == null)
	assert(obj.a == "value")
	return true


func test_property__set() -> bool:
	var obj = test_class.new()
	assert(obj.rawget("_set_some_property") == null)
	obj._set_some_property = "_set_some_property"
	assert(obj.rawget("_set_some_property") == "_set_some_property")
	return true


func test_method() -> bool:
	var obj = test_class.new()
	assert(obj.echo() == null)
	assert(obj.echo(1) == 1)
	assert(obj.echo("hello!") == "hello!")
	assert(obj.echo([3]) == [3])
	var arr = [3]
	assert(is_same(obj.echo(arr), arr))
	assert(obj.echo is Callable)
	assert(obj.echo.bind("callable").call() == "callable")
	return true
