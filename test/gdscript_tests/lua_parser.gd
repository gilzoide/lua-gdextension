extends RefCounted


func test_parse_errors() -> bool:
	var parser = LuaParser.new()
	# parser.debug_log = true

	var tree = parser.parse_code("hello = =")
	assert(tree is LuaAST)
	assert(tree.has_errors())
	# print(tree.root.dump())
	
	var test_script = FileAccess.open("res://gdscript_tests/lua_files/test_class.lua", FileAccess.READ)
	var code = test_script.get_as_text()
	tree = parser.parse_code(code)
	assert(tree is LuaAST)
	assert(not tree.has_errors())
	# print(tree.root.dump())
	
	var q = tree.root.query("(chunk (return_statement (_) @v))")
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


func test_LuaASTQuery_example() -> bool:
	# Example code
	var lua_code = """
		local some_int = 42
		some_global = "hello world!"

		return ...
	"""
	# First we need to parse the code
	var lua_ast = LuaParser.new().parse_code(lua_code)
	assert(not lua_ast.has_errors())
	# Example query: find all identifiers used by the Lua code
	var query = lua_ast.root.query("(identifier) @capture")
	for m in query:
		# A match is always an Array of captures
		# If the query has no captures, the Array will be empty
		assert(m is Array[LuaASTNode])
		for capture in m:
			# Captures are references for the captured LuaASTNode
			assert(capture is LuaASTNode)
			# Use LuaASTNode.get_source_code to get the content
			var identifier = capture.get_source_code()
			prints("Found identifier in Lua code:", identifier)
	return true
