extends RefCounted


func test_parse_errors() -> bool:
	var parser = LuaParser.new()
	# parser.debug_log = true

	var tree = parser.parse_code("hello = =")
	assert(tree is LuaAST)
	assert(tree.has_errors())
	# print(tree.dump())
	
	var test_script = FileAccess.open("res://gdscript_tests/lua_files/test_class.lua", FileAccess.READ)
	var code = test_script.get_as_text()
	tree = parser.parse_code(code)
	assert(tree is LuaAST)
	assert(not tree.has_errors())
	# print(tree.dump())
	
	var q = tree.query("(chunk (return_statement (_) @v))")
	assert(q.is_valid())
	for captures in q:
		for it in captures:
			assert(it is LuaASTNode)
			# print(it.dump())

	return true
