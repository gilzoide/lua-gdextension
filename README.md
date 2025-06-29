# Lua GDExtension
[![Godot Asset Library page](https://img.shields.io/static/v1?logo=godotengine&label=asset%20library&color=478CBF&message=0.5.0)](https://godotengine.org/asset-library/asset/2330)
[![Build and Test workflow](https://github.com/gilzoide/lua-gdextension/actions/workflows/build.yml/badge.svg)](https://github.com/gilzoide/lua-gdextension/actions/workflows/build.yml)

<img src="addons/lua-gdextension/icon.png" alt="Lua GDExtension icon" width="150" height="150"/>

Extension for using the [Lua programming language](https://www.lua.org/) in Godot 4.4+

With this addon, you can program your game or application directly in Lua.
You can also create sandboxed Lua states for external modding/scripting support, as many as necessary.

This plugin is available in the Asset Library as [Lua GDExtension](https://godotengine.org/asset-library/asset/2330).


## Features
- Create Godot scripts directly in Lua, making it possible to use Lua as an alternative to GDScript or C#
- Create additional Lua states for external modding/scripting support, as many as necessary
- Manage Lua tables, functions and coroutines directly from GDScript, C# or any other scripting language in Godot
- Select which Lua libraries and Godot APIs will be available per Lua state, making sandboxing easier:
  + Lua libraries are the same as Lua/C, like `base`, `package` and `io` libraries
  + Godot APIs:
    + Create and manipulate Variant values
    + Instantiate objects and access class constants
    + Access singleton objects by name
    + Utility functions, like `print_rich`, `lerp` and `is_same`
    + Global enums, like `OK`, `TYPE_STRING` and `SIDE_LEFT`
    + Patch Lua `package.searchers`, `require`, `loadfile` and `dofile` to accept paths relative to `res://` and `user://`
- Editor plugin with Lua REPL for testing out Lua snippets
- Choose between Lua 5.4 or LuaJIT v2.1 runtimes (distributed as separate addons)
  + Note: LuaJIT does not support WebAssebly, so Lua 5.4 is always used in Web platform


## Lua scripting in Godot
This addon registers a [ScriptLanguageExtension](https://docs.godotengine.org/en/stable/classes/class_scriptlanguageextension.html) so that Godot objects can be scripted directly in Lua.

For Lua scripts to be usable in Nodes and Resources, they must return a table with the script metadata containing methods, properties, signals, etc...
```lua
-- This is our script metadata table.
--
-- It stores metadata such as its base class, global class_name, icon,
-- as well as any declared properties, methods and signals
local LuaBouncingLogo = {
	-- base class (optional, defaults to RefCounted)
	extends = Sprite2D,
	-- if true, allow the script to be executed by the editor (optional)
	tool = false,
	-- global class name (optional)
	class_name = "LuaBouncingLogo",
	
	-- Declare properties
	linear_velocity = export(100),
	initial_angle = export({
		type = float,
		default = 0,
		hint = PROPERTY_HINT_RANGE,
		hint_string = "0,360,degrees"
	}),
	-- Declare signals
	bounced = signal(),
}

-- Called when the node enters the scene tree for the first time.
function LuaBouncingLogo:_ready()
	self.position = self:get_viewport():get_size() / 2
	self.movement = Vector2(self.linear_velocity, 0):rotated(deg_to_rad(self.initial_angle))
end

-- Called every frame. 'delta' is the elapsed time since the previous frame.
function LuaBouncingLogo:_process(delta)
	local viewport_size = self:get_viewport():get_size()
	local viewport_rect = Rect2(Vector2(), viewport_size)
	if not viewport_rect:encloses(self.global_transform * self:get_rect()) then
		self.movement = self.movement:rotated(deg_to_rad(90))
		self.bounced:emit()
	end
	self.position = self.position + self.movement * delta
end

-- Return the metadata table for the script to be usable by Godot objects
return LuaBouncingLogo
```


## Calling Lua from Godot
The following classes are registered in Godot for creating Lua states and interacting with them: `LuaState`, `LuaTable`, `LuaUserdata`, `LuaLightUserdata`, `LuaFunction`, `LuaCoroutine`, `LuaThread`, `LuaDebug` and `LuaError`.

Usage example in GDScript:
```gdscript
# 1. Create a Lua state
var lua = LuaState.new()
# 2. Import Lua and Godot APIs into the state
#    Optionally pass which libraries should be opened to the method
lua.open_libraries()

# 3. Run Lua code using `LuaState.do_string` or `LuaState.do_file`
var result = lua.do_string("""
  local vector = Vector2(1, 2)
  return {
    this_is_a_table = true,
    vector = vector,
  }
""")
# 4. Access results from Lua code directly in Godot
#    When errors occur, instances of `LuaError` will be returned
if result is LuaError:
    printerr("Error in Lua code: ", result)
else:
    print(result)  # [LuaTable:0x556069ee50ab]
    print(result["this_is_a_table"])  # true
    print(result["vector"])  # (1, 2)
    print(result["invalid key"])  # <null>

# 5. Access the global _G table via `LuaState.globals` property
assert(lua.globals is LuaTable)
lua.globals["a_godot_callable"] = func(): print("Hello from GDScript!")
lua.do_string("""
    a_godot_callable()  -- 'Hello from GDScript!'
""")
```


## Calling Godot from Lua
- Instantiate and manipulate Godot objects, just like in GDScript.
  ```lua
  local v = Vector3(1, 2, 3)
  print(v.x)  -- 1
  -- Note: use ":" instead of "." to call methods in Lua
  print(v:length())  -- 3.74165749549866

  local n = Node:new()
  print(n:is_inside_tree())  -- false
  n:queue_free()
  ```
- Call Godot utility functions.
  ```lua
  local d1 = Dictionary()
  local d2 = Dictionary()
  print(is_same(d1, d2))  -- false
  print(is_same(d2, d2))  -- true

  print(lerp(5, 10, 0.15))  -- 5.75
  ```
- Access singleton objects by name.
  ```lua
  assert(OS == Engine:get_singleton("OS"))
  ```
- Construct Array/Dictionary using Lua tables.
  ```lua
  local array = Array{ "value 0", "value 1" }
  -- Godot Arrays are indexed from 0, instead of 1
  print(array[0])  -- "value 0"
  print(array[1])  -- "value 1"
  print(array[2])  -- nil

  local dict = Dictionary{ hello = "world" }
  print(dict)  -- { "hello": "world" }
  print(dict["hello"])  -- "world"
  print(dict["invalid key"])  -- nil
  ```
- Iterate over values using `pairs` for types that support it, like Arrays, packed arrays, Dictionaries and some math types.
  ```lua
  local dictionary = Dictionary{ hello = "world", key = "value" }
  for key, value in pairs(dictionary) do
      print(key .. ": " .. value)
  end
  ```
- Length operator (`#`) as a shortcut for calling the `size()` method in any object that supports it.
  ```lua
  local array = Array{ 1, 2, 3, 4 }
  print(#array)  -- 4
  ```
- Runtime type check using the `Variant.is` method.
  ```lua
  local array = Array()
  print(Variant.is(array, Array))  -- true
  print(Variant.is(array, 'Array'))  -- true
  -- Also available using the method notation from Variant objects
  print(array:is(Array))  -- true
  print(array:is(Dictionary))  -- false
  print(array:is(RefCounted))  -- false
  ```
- Making protected calls using `pcall`.
  ```lua
  local v = Vector2(1, 2)
  print(v:pcall('length'))  -- true    2.2360680103302
  print(v:pcall('invalid method'))  -- false   "Invalid method"
  ```


## TODO
- [X] Bind Variant types to Lua
- [X] Bind utility functions to Lua
- [X] Bind enums and constants to Lua
- [X] Add support for getting global singletons from Lua
- [X] Add support for getting classes from Lua
- [X] Add optional support for `res://` relative paths in `require`, `loadfile` and `dofile`
- [X] Add support for `await`ing signals
- [X] Submit to Asset Library
- [X] Lua ScriptLanguageExtension
  + [X] Add support for property hints / usage flags (including export)
  + [X] Add support for property getter / setter
  + [ ] Add `export_*` functions mimicking GDScript annotations for better UX
- [X] Support for building with LuaJIT
- [X] Support WebAssembly platform
- [X] Support Windows arm64 platform
- [X] Support Linux arm64 platform
- [ ] Support Linux arm32 and rv64 platform
- [X] Use framework in iOS (possibly a xcframework supporting the iOS simulator as well)
- [X] Automated unit tests
- [X] Automated build and distribution
- [X] Lua REPL editor plugin


## Other projects for using Lua in Godot 4
- https://github.com/WeaselGames/godot_luaAPI
- https://github.com/perbone/luascript
