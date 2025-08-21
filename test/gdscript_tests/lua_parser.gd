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
	var it_count = 0
	for captures in q:
		it_count += 1
		for it in captures:
			assert(it is LuaASTNode)
			# print(it.dump())
	
	var all_matches = q.all_matches()
	assert(all_matches.size() == it_count)

	var first_match = q.first_match()
	assert(first_match != null)
	for it in first_match:
		assert(it is LuaASTNode)
		# print(it.dump())

	return true
