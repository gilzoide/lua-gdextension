extends SceneTree

func _initialize():
	var all_success = true
	
	for lua_script in DirAccess.get_files_at("res://lua_tests"):
		var lua_state = LuaState.new()
		lua_state.open_libraries()
		
		var file_name = "res://lua_tests/" + lua_script
		var result = lua_state.do_file(file_name)
		if result is LuaError:
			all_success = false
			printerr("🗴 ", lua_script, ": ", result.message)
		else:
			print("✓ ", lua_script)

	for gdscript in DirAccess.get_files_at("res://gdscript_tests"):
		var file_name = "res://gdscript_tests/" + gdscript
		var obj = load(file_name).new()
		obj.test()
	
	quit(0 if all_success else -1)
