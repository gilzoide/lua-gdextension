# Lua GDExtension
[![Build and Test workflow](https://github.com/gilzoide/lua-gdextension/actions/workflows/build.yml/badge.svg)](https://github.com/gilzoide/lua-gdextension/actions/workflows/build.yml)

<img src="addons/lua-gdextension/icon.png" alt="Lua GDExtension icon" width="128" height="128"/>

Extension for using the Lua programming language in Godot 4.1.2+


## Features
- Run Lua scripts, access Lua tables, functions and coroutines directly from GDScript, C# or any other scripting language in Godot
- Create as many Lua states as you need
- Select which Lua libraries and Godot APIs will be available per Lua state, making sandboxing easier:
  + Lua libraries are the same as Lua/C, like `base`, `package` and `io` libraries
  + Godot APIs:
    + Create and manipulate Variant values
    + Instantiate objects and access class constants
    + Access singleton objects by name
    + Utility classes, like `print`, `lerp` and `is_same`
    + Global enums, like `OK`, `TYPE_STRING` and `SIDE_LEFT`
    + (TODO) Patch Lua `package.path` and `package.cpath` to accept paths relative to `res://` and `user://`
- (TODO) Create Godot scripts directly in Lua


## Calling Lua from Godot
The following classes are registered in Godot for creating Lua states and interacting with them: `LuaState`, `LuaError`, `LuaCoroutine`, `LuaFunction`, `LuaLightUserdata`, `LuaTable` and `LuaUserdata`.

Usage example:
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
- [ ] Submit to Asset Library
- [ ] Lua ScriptLanguageExtension
- [ ] Support for building with LuaJIT
- [X] Automated unit tests
- [X] Automated build and distribution
- [ ] Lua REPL editor plugin


## Other projects for using Lua in Godot 4
- https://github.com/WeaselGames/godot_luaAPI
- https://github.com/perbone/luascript
