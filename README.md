# Lua GDExtension
Godot 4 native extension for using the Lua programming language.


## Features
- Godot classes for creating Lua states and interacting with them: `LuaState`, `LuaError`, `LuaTable`
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
- Dynamic type constructors, so that all Variant constructors are available
  ```lua
  local v = Vector3(1, 2, 3)
  print(v:length())  -- 3.74165749549866
  ```
- Dynamic method calls, so the whole API for types/classes are available.
  Static type methods are also available.
  ```lua
  local b = Basis:from_euler(Vector3(1, 2, 3))
  print(b)  -- "[X: (0.51996, 0.076247, 0.850781), Y: (-0.698763, -0.534895, 0.474991), Z: (0.491295, -0.841471, -0.224845)]"
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
- `Variant` implements all operator metamethods, `__tostring`, custom iteration via `__pairs` for types that support it, length operator `__len` that calls `size()` method in types that support it
- Runtime type check using the `is` method
  ```lua
  local array = Array()
  print(array:is(Array))  -- true
  print(array:is(Dictionary))  -- false
  ```
- `pcall` method for protected calls
  ```lua
  local v = Vector2(1, 2)
  print(v:pcall('length'))  -- true    2.2360680103302
  print(v:pcall('invalid method'))  -- false   "Invalid method"
  ```


## TODO
- [ ] Bind utility functions to Lua
- [ ] Add support for getting global singletons from Lua
- [ ] Add support for getting classes from Lua
- [ ] Lua ScriptLanguageExtension
- [ ] Support for building with LuaJIT
