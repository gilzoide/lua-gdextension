# Lua GDExtension
Godot 4.1+ native extension for using the Lua programming language.


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
- Bindings for all Variant types.
  ```lua
  local v = Vector3(1, 2, 3)
  print(v:length())  -- 3.74165749549866
  ```
- All object methods are available, including static Variant methods.
  They are dispatched dynamically at runtime.
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
- [ ] Add support for getting global singletons from Lua
- [ ] Add support for getting classes from Lua
- [ ] Lua ScriptLanguageExtension
- [ ] Support for building with LuaJIT
