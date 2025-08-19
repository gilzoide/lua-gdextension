extends RefCounted


func test_parse_errors() -> bool:
	var parser = LuaParser.new()
	# parser.debug_log = true

	var tree = parser.parse_code("hello = =")
	assert(tree is LuaAST)
	assert(tree.has_errors())
	# print(tree.dump())
	
	tree = parser.parse_code("return hello")
	assert(tree is LuaAST)
	assert(not tree.has_errors())
	# print(tree.dump())
	return true
