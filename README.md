# Lua GDExtension
Godot 4.1+ native extension for using the Lua programming language.


## Features
- Godot classes for creating Lua states and interacting with them: `LuaState`, `LuaError`, `LuaTable`, `LuaUserdata`
  ```gdscript
  var lua = LuaState.new()
  lua.open_libraries()  # this imports Lua and Godot APIs to the state
  
  var result = lua.do_string("""
    local vector = Vector2(1, 2)
    return {
      this_is_a_table = true,
      vector = vector,
    }
  """)
  if result is LuaError:
      printerr("Error: ", result)
  else:
      print(result)  # [LuaTable:0x556069ee50ab]
      print(result["this_is_a_table"])  # true
      print(result["vector"])  # (1, 2)
      print(result["invalid key"])  # <null>
  ```
- Bindings for all Variant types and classes.
  Methods are dispatched dynamically at runtime, so that all classes are supported, even those registered at runtime like ones implemented via GDExtension.
  ```lua
  local v = Vector3(1, 2, 3)
  print(v:length())  -- 3.74165749549866

  local n = Node:new()
  print(n:is_inside_tree())  -- false
  n:queue_free()
  ```
- Array/Dictionary constructors accepting table arguments
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
- Custom iteration via `__pairs` for types that support it, like Arrays, packed arrays, Dictionaries and some math types.
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
- Runtime type check using the `is` method.
  ```lua
  local array = Array()
  print(array:is(Array))  -- true
  print(array:is(Dictionary))  -- false
  ```
- `pcall` method for protected calls.
  ```lua
  local v = Vector2(1, 2)
  print(v:pcall('length'))  -- true    2.2360680103302
  print(v:pcall('invalid method'))  -- false   "Invalid method"
  ```
- Bindings for all Godot utility functions.
  ```lua
  local d1 = Dictionary()
  local d2 = Dictionary()
  print(is_same(d1, d2))  -- false
  print(is_same(d2, d2))  -- true

  print(lerp(5, 10, 0.15))  -- 5.75
  ```


## TODO
- [X] Bind utility functions to Lua
- [ ] Bind enums and constants to Lua
- [X] Add support for getting global singletons from Lua
- [X] Add support for getting classes from Lua
- [ ] Submit to Asset Library
- [ ] Lua ScriptLanguageExtension
- [ ] Support for building with LuaJIT
- [X] Automated unit tests
- [X] Automated build and distribution
- [ ] Lua REPL editor plugin
